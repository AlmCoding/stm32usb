/*
 * I2c.hpp
 *
 *  Created on: Aug 15, 2023
 *      Author: Alexander L.
 */

#ifndef HAL_I2C_I2CMASTER_HPP_
#define HAL_I2C_I2CMASTER_HPP_

#include "cmsis_os.h"
#include "common.hpp"
#include "main.h"

#define START_I2_REQUEST_IMMEDIATELY false

namespace hal::i2c {

typedef StaticQueue_t osStaticMessageQDef_t;

class I2cMaster {
 private:
  constexpr static size_t RequestQueueSize = 4;
  constexpr static size_t DataBufferSize = 128;

  typedef struct {
    size_t space1;  // Starts at data_end_
    size_t space2;  // Starts at 0
  } Space;

 public:
  typedef struct {
    uint16_t request_id;
    uint16_t slave_addr;
    uint16_t write_size;
    uint16_t read_size;
    size_t write_start;
    size_t read_start;
    uint16_t sequence_id;
    uint16_t sequence_idx;
  } Request;

  I2cMaster(I2C_HandleTypeDef* i2c_handle);
  virtual ~I2cMaster();

  Status_t config();
  Status_t init();
  uint32_t poll();

  Status_t scheduleRequest(Request* request, uint8_t* write_data, uint32_t seq_num);
  Status_t serviceRequest();

 private:
  void getFreeSpace(Space* free);
  int32_t allocateBufferSpace(size_t size);
  Status_t startRequest();
  Status_t startWrite();
  Status_t startReadReg();
  Status_t startRead();
  void writeCpltCb();
  void readCpltCb();

  I2C_HandleTypeDef* i2c_handle_;

  osMessageQueueId_t pending_queue_;
  uint8_t pending_q_buffer_[RequestQueueSize * sizeof(Request)];
  osStaticMessageQDef_t pending_q_ctrl_;
  osMessageQueueAttr_t pending_q_attr_ = {
    .name = "i2cMasterPndQ",
    .cb_mem = &pending_q_ctrl_,
    .cb_size = sizeof(pending_q_ctrl_),
    .mq_mem = &pending_q_buffer_,
    .mq_size = sizeof(pending_q_buffer_),
  };

  osMessageQueueId_t complete_queue_;
  uint8_t complete_q_buffer_[RequestQueueSize * sizeof(Request)];
  osStaticMessageQDef_t complete_q_ctrl_;
  osMessageQueueAttr_t complete_q_attr_ = {
    .name = "i2cMasterCltQ",
    .cb_mem = &complete_q_ctrl_,
    .cb_size = sizeof(complete_q_ctrl_),
    .mq_mem = &complete_q_buffer_,
    .mq_size = sizeof(complete_q_buffer_),
  };

  uint8_t data_buffer_[DataBufferSize];
  size_t data_start_ = 0;
  size_t data_end_ = 0;

  bool request_rejected_ = false;

  Request request_;
  uint32_t xfer_options_ = I2C_FIRST_AND_LAST_FRAME;
  bool request_complete_ = true;

  bool send_data_msg_ = false;
  bool send_status_msg_ = false;
  uint32_t seqence_number_ = 0;

  friend class I2cIrq;
};

}  // namespace hal::i2c

#endif /* HAL_I2C_I2CMASTER_HPP_ */
