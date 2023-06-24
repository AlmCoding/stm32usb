# Communication Protocol

For managing data frames the TinyFrame (https://github.com/MightyPork/TinyFrame) library is used.
Its header specifies a frame type which is used to identify the peripheral task to which the message shall be forwarded. 

``` cpp
enum class UsbMsgType : uint8_t {
  CtrlMsg = 0,
  UartMsg,
  I2cMsg,
  SpiMsg,
  CanMsg,
  PwmMsg,
  GpioMsg,
  AdcMsg,
  // Keep this at the end
  NumValues,
};
```

This frame payload is then forwared to its target task. The forwarding happens in the context of the USB RX interrupt.
The first palyload byte specifies the format of the following bytes:


The payload is forated in json


# Uart concept


## RX Part
DMA is writing incomming bytes in ring buffer
- cyclic check of how many bytes are in buffer
- RX complete interrupt shall trigger task for cyclic check (later)
