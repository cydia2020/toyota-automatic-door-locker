#include <SPI.h>
#include "mcp2515_can.h"

const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

mcp2515_can CAN(SPI_CS_PIN);

bool flagRecv = 0;
unsigned char len = 0;
unsigned char combinationMeterBuffer[8];
unsigned char lockCommand[8] = {0x40, 0x05, 0x30, 0x11, 0x00, 0x40, 0x00, 0x00};
unsigned char unlockCommand[8] = {0x40, 0x05, 0x30, 0x11, 0x00, 0x80, 0x00, 0x00};
char str[20];
bool doorLocked = 0;

void setup() {
  pinMode(A1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), MCP2515_ISR, FALLING);
  CAN.begin(CAN_500KBPS);

  CAN.init_Mask(0, 0, 0x3ff);
  CAN.init_Mask(1, 0, 0x3ff);

  CAN.init_Filt(0, 0, 0x620);
}

void MCP2515_ISR() {
  flagRecv = true;
}

void loop() {
  if (flagRecv) {
    flagRecv = false;
    CAN.readMsgBuf(&len, combinationMeterBuffer);

    // lock command is on 0x750
    if (bitRead(combinationMeterBuffer[7], 4) == 0 && !doorLocked) {
      CAN.sendMsgBuf(0x750, 0, 8, lockCommand);
      doorLocked = 1;
    }
    if (bitRead(combinationMeterBuffer[7], 4) == 1 && doorLocked) {
      CAN.sendMsgBuf(0x750, 0, 8, unlockCommand);
      doorLocked = 0;
    }

    delay(125/11); // don't spam can
  }
}
