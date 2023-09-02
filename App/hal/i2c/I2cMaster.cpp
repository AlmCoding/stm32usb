/*
 * I2c.cpp
 *
 *  Created on: Aug 15, 2023
 *      Author: Alexander L.
 */

#include "hal/i2c/I2cMaster.hpp"
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
  return Status_t::Ok;
}

Status_t I2cMaster::init() {
  osMessageQueueReset(pending_queue_);
  osMessageQueueReset(complete_queue_);
  this_complete_ = true;
  return Status_t::Ok;
}

uint32_t I2cMaster::poll() {
  uint32_t service_requests = 0;

#if (START_I2_REQUEST_IMMEDIATELY == false)
  // startTx();
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
  int32_t start;

  if (osMessageQueueGetSpace(pending_queue_) == 0) {
    DEBUG_ERROR("Queue overflow (seq: %d)", seq_num);
    queue_overflow_ = true;
    send_status_msg_ = true;
    return Status_t::Error;
  }

  if (data_start_ == data_end_) {
    data_start_ = 0;
    data_end_ = 0;
  }

  if (request->type == RequestType::Write) {
    // Write request
    start = allocateBufferSpace(request->write.write_size);
    if (start >= 0) {
      request->write.write_start = start;
      std::memcpy(data_buffer_ + start, write_data, request->write.write_size);
    }

  } else {
    // Read request
    start = allocateBufferSpace(request->read.read_size);
    if (start >= 0) {
      request->read.read_start = start;
    }
  }

  if (start == -1) {
    DEBUG_ERROR("Buffer overflow (seq: %d)", seq_num);
    buffer_overflow_ = true;
    send_status_msg_ = true;
    return Status_t::Error;
  }

  // Add request to queue
  if (osMessageQueuePut(pending_queue_, request, 0, 0) == osOK) {
    status = Status_t::Ok;

#if (START_I2_REQUEST_IMMEDIATELY == true)
    // startTx();
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

}  // namespace hal::i2c
