/*
 * I2c.cpp
 *
 *  Created on: Aug 15, 2023
 *      Author: Alexander L.
 */

#include "hal/i2c/I2cMaster.hpp"

namespace hal::i2c {

I2cMaster::I2cMaster(I2C_HandleTypeDef* i2c_handle) : i2c_handle_{ i2c_handle } {
  queue_handle_ = osMessageQueueNew(RequestQueueSize, sizeof(MasterRequest), &queue_attr_);
}

I2cMaster::~I2cMaster() {}

Status_t I2cMaster::config() {
  return Status_t::Ok;
}

Status_t I2cMaster::init() {
  return Status_t::Ok;
}

uint32_t I2cMaster::poll() {
  return 0;
}

Status_t I2cMaster::scheduleRequest(MasterRequest* request, uint8_t* write_data) {
  Status_t status = Status_t::Error;

  if (data_start_ == data_end_) {
    data_start_ = 0;
    data_end_ = 0;
  }

  // Write data
  auto start = allocateBufferSpace(request->write_size);
  if (start >= 0) {
    request->write_start = start;
    std::memcpy(data_buffer_ + request->write_start, write_data, request->write_size);
  } else {
    return Status_t::Error;
  }

  // Read data
  start = allocateBufferSpace(request->read_size);
  if (start >= 0) {
    request->read_start = start;
  } else {
    return Status_t::Error;
  }

  // Add request to queue
  if (osMessageQueuePut(queue_handle_, request, 0, 0) == osOK) {
    status = Status_t::Ok;
  }

  return status;
}

Status_t I2cMaster::serviceRequest() {
  return Status_t::Ok;
}

void I2cMaster::getFreeSpace(FreeSpace* free) {
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

  FreeSpace free;
  getFreeSpace(&free);

  if (free.space1 >= size) {
    start = data_end_;
    data_end_ += size;

  } else if (free.space2 >= size) {
    start = 0;
    data_end_ = size;

  } else {
    start = -1;
  }

  return start;
}

}  // namespace hal::i2c
