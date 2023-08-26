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
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][i2cSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][i2cSrv]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][i2cSrv]: " f "\n", ##__VA_ARGS__);
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

void I2cService::poll() {}

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

  if (i2c_msg.which_msg == i2c_proto_I2cMsg_master_tag) {
    hal::i2c::MasterRequest request;

    request.request_id = static_cast<uint16_t>(i2c_msg.msg.master.request_id);
    request.slave_addr = static_cast<uint16_t>(i2c_msg.msg.master.slave_addr);
    request.write_size = static_cast<uint16_t>(i2c_msg.msg.master.data.size);
    request.read_size = static_cast<uint16_t>(i2c_msg.msg.master.read_size);
    request.completed = false;

    if (i2cMaster0_.scheduleRequest(&request, static_cast<uint8_t*>(i2c_msg.msg.master.data.bytes)) == Status_t::Ok) {
      status = 0;
    }

  } else if (i2c_msg.which_msg == i2c_proto_I2cMsg_slave_tag) {
    status = 0;

  } else if (i2c_msg.which_msg == i2c_proto_I2cMsg_cfg_tag) {
    status = 0;
  }

  return status;
}

int32_t I2cService::serviceRequest(uint8_t* data, size_t max_len) {
  return 0;
}

}  // namespace app::i2c_srv
