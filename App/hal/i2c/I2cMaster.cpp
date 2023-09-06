/*
 * I2c.cpp
 *
 *  Created on: Aug 15, 2023
 *      Author: Alexander L.
 */

#include "hal/i2c/I2cMaster.hpp"
#include "hal/i2c/I2cIrq.hpp"
#include "os/msg/msg_broker.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_I2C_MASTER
#ifdef DEBUG_ENABLE_I2C_MASTER
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][I2cMstr]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][I2cMstr]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][I2cMstr]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace hal::i2c {

I2cMaster::I2cMaster(I2C_HandleTypeDef* i2c_handle) : i2c_handle_{ i2c_handle } {
  pending_queue_ = osMessageQueueNew(RequestQueueSize, sizeof(I2cMaster::Request), &pending_q_attr_);
  complete_queue_ = osMessageQueueNew(RequestQueueSize, sizeof(I2cMaster::Request), &complete_q_attr_);
}

I2cMaster::~I2cMaster() {}

Status_t I2cMaster::config() {
  I2cIrq::getInstance().registerI2cMaster(this);
  return Status_t::Ok;
}

Status_t I2cMaster::init() {
  osMessageQueueReset(pending_queue_);
  osMessageQueueReset(complete_queue_);
  request_complete_ = true;
  request_rejected_ = false;
  return Status_t::Ok;
}

uint32_t I2cMaster::poll() {
  uint32_t service_requests = 0;

#if (START_I2_REQUEST_IMMEDIATELY == false)
  startRequest();
#endif

  if (osMessageQueueGetCount(complete_queue_) > 0) {
    send_data_msg_ = true;
    service_requests++;
  }

  if (send_status_msg_ == true) {
    service_requests++;
  }

  return service_requests;
}

Status_t I2cMaster::scheduleRequest(Request* request, uint8_t* write_data, uint32_t seq_num) {
  Status_t status = Status_t::Error;
  int32_t start = 0;

  // Check queue space
  if (osMessageQueueGetSpace(pending_queue_) == 0) {
    DEBUG_ERROR("Queue overflow (seq: %d)", seq_num);
    request_rejected_ = true;
    send_status_msg_ = true;
    return Status_t::Error;
  }

  if (data_start_ == data_end_) {
    data_start_ = 0;
    data_end_ = 0;
  }

  // Write space
  if (request->write_size > 0) {
    start = allocateBufferSpace(request->write_size);
    if (start >= 0) {
      request->write_start = start;
      std::memcpy(data_buffer_ + start, write_data, request->write_size);
    }
  }

  // Read space
  if ((request->read_size > 0) && (start >= 0)) {
    start = allocateBufferSpace(request->read_size);
    if (start >= 0) {
      request->read_start = start;
    }
  }

  if (start < 0) {
    DEBUG_ERROR("Buffer overflow (seq: %d)", seq_num);
    request_rejected_ = true;
    send_status_msg_ = true;
    return Status_t::Error;
  }

  // Add request to pending_queue
  if (osMessageQueuePut(pending_queue_, request, 0, 0) == osOK) {
    status = Status_t::Ok;

#if (START_I2_REQUEST_IMMEDIATELY == true)
    startRequest();
#else
    // Trigger i2c task for fast transmit start
    os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
    os::msg::send_msg(os::msg::MsgQueue::I2cTaskQueue, &msg);
#endif
  }

  seqence_number_ = seq_num;
  return status;
}

Status_t I2cMaster::serviceRequest() {
  return Status_t::Ok;
}

void I2cMaster::getFreeSpace(Space* free) {
  if (data_start_ < data_end_) {
    free->space1 = data_start_;
    free->space2 = sizeof(data_buffer_) - data_end_;

  } else if (data_start_ > data_end_) {
    free->space1 = data_start_ - data_end_;
    free->space2 = 0;

  } else {
    free->space1 = sizeof(data_buffer_);
    free->space2 = 0;
  }
}

int32_t I2cMaster::allocateBufferSpace(size_t size) {
  int32_t start;

  Space free;
  getFreeSpace(&free);

  if (free.space1 >= size) {
    start = data_end_;
    data_end_ += size;

  } else if (free.space2 >= size) {
    start = 0;
    data_end_ = size;

  } else {
    DEBUG_ERROR("Allocate space failed (len: %d)", size);
    start = -1;
  }

  return start;
}

Status_t I2cMaster::startRequest() {
  Status_t status = Status_t::Ok;
  HAL_I2C_StateTypeDef state = HAL_I2C_GetState(i2c_handle_);
  uint32_t space = osMessageQueueGetSpace(complete_queue_);

  if ((request_complete_ == false) ||  // Request ongoing
      (space == 0) ||                  // Full complete queue
      (state == HAL_I2C_STATE_BUSY)) {
    // Ongoing request or full complete queue or busy i2c
    return Status_t::Busy;
  }

  if (osMessageQueueGet(pending_queue_, &request_, 0, 0) == osOK) {
    // Request taken from queue
    if ((request_.write_size > 0) && (request_.read_size == 0)) {
      // Write only
      status = startWrite();
    } else if ((request_.read_size > 0) && (request_.write_size == 0)) {
      // Read only
      status = startRead();
    } else if ((request_.read_size > 0) && (request_.write_size <= 2)) {
      // Read register
      status = startReadReg();
    } else if ((request_.read_size > 0) && (request_.write_size > 0)) {
      // Write + read
      status = startWrite();
    } else {
      DEBUG_ERROR("Invalid read & write size (== 0) (id: %d)", request_.request_id);
      status = Status_t::Error;
    }

    status = startRequest();
  }

  return status;
}

Status_t I2cMaster::startWrite() {
  Status_t status;
  HAL_StatusTypeDef hal_status;

  uint16_t slave_addr = request_.slave_addr << 1;

  hal_status = HAL_I2C_Master_Seq_Transmit_DMA(i2c_handle_,                          // Handle
                                               slave_addr,                           // Addr
                                               data_buffer_ + request_.write_start,  // Data
                                               request_.write_size,                  // Length
                                               xfer_options_);
  if (hal_status == HAL_OK) {
    DEBUG_INFO("Start write (id: %d) [ok]", request_.request_id);
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Start write (id: %d) [failed]", request_.request_id);
    status = Status_t::Error;
  }

  return status;
}

Status_t I2cMaster::startReadReg() {
  Status_t status;
  HAL_StatusTypeDef hal_status;

  uint16_t slave_addr = request_.slave_addr << 1;
  uint16_t reg_addr = *(uint16_t*)(data_buffer_ + request_.write_start);

  hal_status = HAL_I2C_Mem_Read_DMA(i2c_handle_,                         // Handle
                                    slave_addr,                          // Slave addr
                                    reg_addr,                            // Register addr
                                    request_.write_size,                 // Addr size
                                    data_buffer_ + request_.read_start,  // Ptr
                                    request_.read_size);

  if (hal_status == HAL_OK) {
    DEBUG_INFO("Start read reg (id: %d) [ok]", request_.request_id);
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Start read reg (id: %d) [failed]", request_.request_id);
    status = Status_t::Error;
  }

  return status;
}

Status_t I2cMaster::startRead() {
  Status_t status;
  HAL_StatusTypeDef hal_status;

  if (hal_status == HAL_OK) {
    DEBUG_INFO("Start read (id: %d) [ok]", request_.request_id);
    status = Status_t::Ok;

  } else {
    DEBUG_ERROR("Start read (id: %d) [failed]", request_.request_id);
    status = Status_t::Error;
  }

  return status;
}

void I2cMaster::writeCpltCb() {
  DEBUG_INFO("Write cplt (id: %d)", request_.request_id);

  // Add request to complete_queue
  if (osMessageQueuePut(complete_queue_, &request_, 0, 0) != osOK) {
    DEBUG_ERROR("Complete queue put (id: %d) [failed]", request_.request_id);
  }

  request_complete_ = true;
  send_status_msg_ = true;

  // Trigger i2c task for fast service notification
  os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
  os::msg::send_msg(os::msg::MsgQueue::I2cTaskQueue, &msg);
}

void I2cMaster::readCpltCb() {
  DEBUG_INFO("Read cplt (id: %d)", request_.request_id);

  // Add request to complete_queue
  if (osMessageQueuePut(complete_queue_, &request_, 0, 0) != osOK) {
    DEBUG_ERROR("Complete queue put (id: %d) [failed]", request_.request_id);
  }

  request_complete_ = true;
  send_status_msg_ = true;

  // Trigger i2c task for fast service notification
  os::msg::BaseMsg msg = { .id = os::msg::MsgId::TriggerTask };
  os::msg::send_msg(os::msg::MsgQueue::I2cTaskQueue, &msg);
}

}  // namespace hal::i2c
