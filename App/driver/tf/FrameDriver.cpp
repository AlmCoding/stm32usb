/*
 * FrameDriver.cpp
 *
 *  Created on: 24 May 2023
 *      Author: Alexander L.
 */

#include "driver/tf/FrameDriver.hpp"
#include "srv/debug.hpp"

#define DEBUG_ENABLE_FRAME_DRIVER
#ifdef DEBUG_ENABLE_FRAME_DRIVER
#define DEBUG_INFO(f, ...) srv::debug::print(srv::debug::TERM0, "[INF][FrameDriver]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::debug::print(srv::debug::TERM0, "[WRN][FrameDriver]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::debug::print(srv::debug::TERM0, "[ERR][FrameDriver]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

namespace driver::tf {

TF_Result typeCallback(TinyFrame* tf, TF_Msg* msg);
TF_Result genericCallback(TinyFrame* tf, TF_Msg* msg);

FrameDriver::FrameDriver() {
  TF_InitStatic(&tf_, TF_SLAVE);
  TF_AddGenericListener(&tf_, genericCallback);
}

StatusType FrameDriver::registerTxCallback(app::usb::UsbMsgType type, app::usb::TxCallback callback) {
  tx_callbacks_[static_cast<uint8_t>(type)] = callback;
  return StatusType::Ok;
}

void FrameDriver::callTxCallback(app::usb::UsbMsgType type) {
  TF_Msg msg = { 0 };

  msg.type = static_cast<TF_TYPE>(type);
  msg.len = static_cast<TF_LEN>(tx_callbacks_[static_cast<uint8_t>(type)](tx_buffer_, sizeof(tx_buffer_)));
  msg.data = tx_buffer_;

  if (msg.len > 0) {
    TF_Send(&tf_, &msg);
  }
}

StatusType FrameDriver::registerRxCallback(app::usb::UsbMsgType type, app::usb::RxCallback callback) {
  StatusType status = StatusType::Error;

  if (TF_AddTypeListener(&tf_, static_cast<uint8_t>(type), typeCallback) == true) {
    rx_callbacks_[static_cast<uint8_t>(type)] = callback;
    status = StatusType::Ok;
  }

  return status;
}

void FrameDriver::callRxCallback(app::usb::UsbMsgType type, const uint8_t* data, size_t size) {
  rx_callbacks_[static_cast<uint8_t>(type)](data, size);
}

void FrameDriver::receiveData(const uint8_t* data, size_t size) {
  TF_Accept(&tf_, data, size);
}

void FrameDriver_receiveData(const uint8_t* data, size_t size) {
  driver::tf::FrameDriver& frameDriver = driver::tf::FrameDriver::getInstance();
  frameDriver.receiveData(data, size);
}

TF_Result typeCallback(TinyFrame* /*tf*/, TF_Msg* msg) {
  driver::tf::FrameDriver& frameDriver = driver::tf::FrameDriver::getInstance();
  frameDriver.callRxCallback(static_cast<app::usb::UsbMsgType>(msg->type), msg->data, msg->len);
  return TF_STAY;
}

TF_Result genericCallback(TinyFrame* /*tf*/, TF_Msg* msg) {
  DEBUG_WARN("Unknown msg type: %d", msg->type);
  return TF_STAY;
}

}  // namespace driver::tf
