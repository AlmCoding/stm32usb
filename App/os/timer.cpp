/*
 * timer_cfg.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Alexander L.
 */

#include "os/timer.hpp"

namespace os {

/* Definitions for testTimer */
static osTimerId_t testTimerHandle;
static osStaticTimerDef_t testTimerControlBlock;
static const osTimerAttr_t testTimer_attributes = {
  .name = "testTimer",
  .cb_mem = &testTimerControlBlock,
  .cb_size = sizeof(testTimerControlBlock),
};

/* Definitions for oneShotTimer */
static osTimerId_t oneShotTimerHandle;
static osStaticTimerDef_t oneShotTimerControlBlock;
static const osTimerAttr_t oneShotTimer_attributes = {
  .name = "oneShotTimer",
  .cb_mem = &oneShotTimerControlBlock,
  .cb_size = sizeof(oneShotTimerControlBlock),
};

/* testTimerCallback function */
void testTimerCallback(void* argument) {}

/* oneShotTimerCallback function */
void oneShotTimerCallback(void* argument) {}

void createTimers() {
  /* Create the timer(s) */
  /* creation of testTimer */
  testTimerHandle = osTimerNew(testTimerCallback, osTimerPeriodic, NULL, &testTimer_attributes);

  /* creation of oneShotTimer */
  oneShotTimerHandle = osTimerNew(oneShotTimerCallback, osTimerOnce, NULL, &oneShotTimer_attributes);
}

}  // namespace os
