/*
 * I2cService.cpp
 *
 *  Created on: Aug 18, 2023
 *      Author: Alexander L.
 */

#include "app/i2c_srv/I2cService.hpp"
#include "os/msg/msg_broker.hpp"
#include "srv/debug.hpp"

#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "proto_c/i2c.pb.h"

#define DEBUG_ENABLE_UART_SERVICE
#ifdef DEBUG_ENABLE_UART_SERVICE
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][I2cSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][I2cSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][I2cSrv]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace app::i2c_srv {

I2cService::I2cService() {}

I2cService::~I2cService() {}

void I2cService::init(app::ctrl::RequestSrvCallback request_service_cb) {
  // i2cMaster0_.config(DefaultClockRate);

  request_service_cb_ = request_service_cb;
}

void I2cService::poll() {
  uint32_t request_cnt = i2cMaster0_.poll();

  if (request_cnt > 0) {
    request_service_cb_(request_cnt);
  }
}

int32_t I2cService::postRequest(const uint8_t* data, size_t len) {
  int32_t status = -1;

  /* Allocate space for the decoded message. */
  i2c_proto_I2cMsg i2c_msg = i2c_proto_I2cMsg_init_zero;
  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(data, len);

  /* Now we are ready to decode the message. */
  if (pb_decode(&stream, i2c_proto_I2cMsg_fields, &i2c_msg) == false) {
    DEBUG_ERROR("ProtoBuf decode [failed]");
    return -1;
  }

  if (i2c_msg.which_msg == i2c_proto_I2cMsg_master_write_tag) {
    status = postMasterWriteRequest(&i2c_msg);

  } else if (i2c_msg.which_msg == i2c_proto_I2cMsg_master_read_tag) {
    status = postMasterReadRequest(&i2c_msg);

  } else if (i2c_msg.which_msg == i2c_proto_I2cMsg_cfg_tag) {
    status = 0;
  }

  return status;
}

int32_t I2cService::postMasterWriteRequest(i2c_proto_I2cMsg* msg) {
  int32_t status = -1;
  hal::i2c::I2cMaster::Request request;

  request.type = hal::i2c::I2cMaster::RequestType::Write;
  request.write.request_id = static_cast<uint16_t>(msg->msg.master_write.request_id);
  request.write.slave_addr = static_cast<uint16_t>(msg->msg.master_write.slave_addr);
  request.write.write_size = static_cast<uint16_t>(msg->msg.master_write.write_data.size);
  request.write.send_stop = static_cast<uint16_t>(msg->msg.master_write.send_stop);

  if (i2cMaster0_.scheduleRequest(&request, static_cast<uint8_t*>(msg->msg.master_write.write_data.bytes),
                                  msg->sequence_number) == Status_t::Ok) {
    status = 0;
  }

  return status;
}

int32_t I2cService::postMasterReadRequest(i2c_proto_I2cMsg* msg) {
  int32_t status = -1;
  hal::i2c::I2cMaster::Request request;

  request.type = hal::i2c::I2cMaster::RequestType::Read;
  request.read.request_id = static_cast<uint16_t>(msg->msg.master_read.request_id);
  request.read.slave_addr = static_cast<uint16_t>(msg->msg.master_read.slave_addr);
  request.read.reg_addr = static_cast<uint16_t>(msg->msg.master_read.reg_addr);
  request.read.send_stop = static_cast<uint16_t>(msg->msg.master_read.send_stop);

  switch (msg->msg.master_read.addr_size) {
    case i2c_proto_AddrSize::i2c_proto_AddrSize_TWO_BYTES: {
      request.read.addr_size = hal::i2c::I2cMaster::AddrSize::TwoBytes;
      break;
    }
    case i2c_proto_AddrSize::i2c_proto_AddrSize_ONE_BYTE: {
      request.read.addr_size = hal::i2c::I2cMaster::AddrSize::OneByte;
      break;
    }
    case i2c_proto_AddrSize::i2c_proto_AddrSize_ZERO_BYTES: {
      request.read.addr_size = hal::i2c::I2cMaster::AddrSize::ZeroBytes;
      break;
    }
    default: {
      DEBUG_ERROR("Invalid MasterReadRequest (addr_size: %i)", msg->msg.master_read.addr_size);
      return -1;
    }
  }

  if (i2cMaster0_.scheduleRequest(&request, nullptr, msg->sequence_number) == Status_t::Ok) {
    status = 0;
  }

  return status;
}

int32_t I2cService::serviceRequest(uint8_t* data, size_t max_len) {
  return 0;
}

}  // namespace app::i2c_srv
