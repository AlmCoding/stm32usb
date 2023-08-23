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

  // TODO set start addresses
  // request->write_start = ;
  // request->read_start = ;

  if (osMessageQueuePut(queue_handle_, request, 0, 0) == osOK) {
    status = Status_t::Ok;
  }

  return status;
}

Status_t I2cMaster::serviceRequest() {
  return Status_t::Ok;
}

}  // namespace hal::i2c
