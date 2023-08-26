extern "C" {

#include "srv/Timeout.hpp"
#include "srv/debug.hpp"
#include "tf/TinyFrame.h"
#include "usbd_cdc_if.h"

#define DEBUG_ENABLE_MAIN
#ifdef DEBUG_ENABLE_MAIN
#define DEBUG_INFO(f, ...) srv::dbg::print(srv::dbg::TERM0, "[INF][TF_int]: " f "\n", ##__VA_ARGS__);
#define DEBUG_WARN(f, ...) srv::dbg::print(srv::dbg::TERM0, "[WRN][TF_int]: " f "\n", ##__VA_ARGS__);
#define DEBUG_ERROR(f, ...) srv::dbg::print(srv::dbg::TERM0, "[ERR][TF_int]: " f "\n", ##__VA_ARGS__);
#else
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif

/**
 * This is an example of integrating TinyFrame into the application.
 *
 * TF_WriteImpl() is required, the mutex functions are weak and can
 * be removed if not used. They are called from all TF_Send/Respond functions.
 *
 * Also remember to periodically call TF_Tick() if you wish to use the
 * listener timeout feature.
 */

void TF_WriteImpl(TinyFrame* /*tf*/, const uint8_t* buff, uint32_t len) {
  srv::Timeout timeout{};
  bool notified = false;

  while (CDC_IsTransmit_Busy() == 1) {
    if (notified == false) {
      timeout.start(Time10ms);
      DEBUG_INFO("USB tx busy ...");
      notified = true;

    } else if (timeout.isExpired() == true) {
      DEBUG_ERROR("USB tx timeout expired! (%d)", timeout.remaining());
      break;
    }
  }

  CDC_Transmit_FS(const_cast<uint8_t*>(buff), static_cast<uint16_t>(len));
}

// --------- Mutex callbacks ----------
// Needed only if TF_USE_MUTEX is 1 in the config file.
// DELETE if mutex is not used

/*
// Claim the TX interface before composing and sending a frame
bool TF_ClaimTx(TinyFrame* tf) {
  // take mutex
  return true;  // we succeeded
}

// Free the TX interface after composing and sending a frame
void TF_ReleaseTx(TinyFrame* tf){
  // release mutex
}
*/

// --------- Custom checksums ---------
// This should be defined here only if a custom checksum type is used.
// DELETE those if you use one of the built-in checksum types

/*
// Initialize a checksum
TF_CKSUM TF_CksumStart(void) {
  return 0;
}

// Update a checksum with a byte
TF_CKSUM TF_CksumAdd(TF_CKSUM cksum, uint8_t byte) {
  return cksum ^ byte;
}

// Finalize the checksum calculation
TF_CKSUM TF_CksumEnd(TF_CKSUM cksum) {
  return cksum;
}
*/

}  // extern "C"
