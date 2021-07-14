#include <SPI.h>
#include "mcp2515_can.h"

const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

mcp2515_can CAN(SPI_CS_PIN);

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char gearPacketBuf[8];
unsigned char lockCommand[8] = {0x40, 0x05, 0x30, 0x11, 0x00, 0x40, 0x00, 0x00};
unsigned char unlockCommand[8] = {0x40, 0x05, 0x30, 0x11, 0x00, 0x80, 0x00, 0x00};
char str[20];

void setup() {
  pinMode(A1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), MCP2515_ISR, FALLING); // start interrupt
  CAN.begin(CAN_500KBPS);

  CAN.init_Mask(0, 0, 0x3ff);
  CAN.init_Mask(1, 0, 0x3ff);

  CAN.init_Filt(0, 0, 0x127);
}

void MCP2515_ISR() {
  flagRecv = 1;
}

void loop() {
  flagRecv = 0;
  CAN.readMsgBuf(&len, gearPacketBuf);

// lock command is on 0x750
  if (gearPacketBuf[5] == 0x30) {
    CAN.sendMsgBuf(0x750, 0, 8, lockCommand);
  }
  if (gearPacketBuf[5] == 0x00) {
    CAN.sendMsgBuf(0x750, 0, 8, unlockCommand);
  }
}
