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

  CAN.init_Mask(0, 0, 0x3ff);                         // there are 2 mask in mcp2515, you need to set both of them
  CAN.init_Mask(1, 0, 0x3ff);

  CAN.init_Filt(0, 0, 0x3BC);                          // toyota universal gear and drive mode address
}

void MCP2515_ISR() {
  flagRecv = 1;
}

void loop() {
  flagRecv = 0;
  CAN.readMsgBuf(&len, gearPacketBuf);

// gear signals are on 0x3bc, park signal is on the 13th bit
// while drive is on the 47th bit, counting from zero

// lock command is on 0x750

// send lock command when shifting to drive, 47th bit is the 8th bit of line 6
  if (bitRead(gearPacketBuf[5], 7) == 1) {
    CAN.sendMsgBuf(0x750, 0, 8, lockCommand);
  }

// send unlock command when shifting to park, 13th bit is the 6th bit of line 2
  if (bitRead(gearPacketBuf[1], 5) == 0) {
    CAN.sendMsgBuf(0x750, 0, 8, unlockCommand);
  }
}
