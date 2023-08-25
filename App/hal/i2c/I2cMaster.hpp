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

namespace hal::i2c {

typedef StaticQueue_t osStaticMessageQDef_t;

constexpr size_t RequestQueueSize = 8;
constexpr size_t DataBufferSize = 128;

typedef struct {
  uint16_t request_id;
  uint16_t slave_addr;
  uint16_t write_size;
  uint16_t read_size;
  size_t write_start;
  size_t read_start;
  bool completed;
} MasterRequest;

class I2cMaster {
  typedef struct {
    size_t space1;
    size_t space2;
  } FreeSpace;

 public:
  I2cMaster(I2C_HandleTypeDef* i2c_handle);
  virtual ~I2cMaster();

  Status_t config();
  Status_t init();
  uint32_t poll();

  Status_t scheduleRequest(MasterRequest* request, uint8_t* write_data);
  Status_t serviceRequest();

 private:
  void getFreeSpace(FreeSpace* free);
  int32_t allocateBufferSpace(size_t size);

  I2C_HandleTypeDef* i2c_handle_;
  osMessageQueueId_t queue_handle_;

  uint8_t queue_buffer_[RequestQueueSize * sizeof(MasterRequest)];
  osStaticMessageQDef_t queue_ctrl_;
  osMessageQueueAttr_t queue_attr_ = {
    .name = "i2cMasterQueue",
    .cb_mem = &queue_ctrl_,
    .cb_size = sizeof(queue_ctrl_),
    .mq_mem = &queue_buffer_,
    .mq_size = sizeof(queue_buffer_),
  };

  uint8_t data_buffer_[DataBufferSize];
  size_t data_start_ = 0;
  size_t data_end_ = 0;

  uint32_t seqence_number_ = 0;

  // friend class I2cIrq;
};

}  // namespace hal::i2c

#endif /* HAL_I2C_I2CMASTER_HPP_ */
