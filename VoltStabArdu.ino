#define CLK 2
#define DIO 3

#include "StabDisplay.h"
StabDisplay disp(CLK, DIO);

uint32_t Now, clocktimer;
boolean flag;

void setup() {
  Serial.begin(9600);
  disp.clear();
  disp.brightness(5);  // яркость, 0 - 7 (минимум - максимум)
  countDown();
}

void loop() {
  disp.displayInputVoltage(230);
  disp.greenLedOn();
  delay(2);
  disp.displayOutputVoltage(250);
  // disp.yellowLedOn();
  // disp.redLedOn();
  // disp.displayByte(0x05, 0xfe);
  // disp.displayByte(0x04, 0xfe);
  // disp.displayByte(0x03, 0xfe);
  // disp.displayByte(0x02, 0xfe);
  // disp.displayByte(0x01, 0xfe);
  // disp.displayByte(0x00, 0xfe);
  // delay(2);
}

void countDown() {
  disp.clear();
  for (int i = 9; i >=0; i--) {
    disp.displayByte(0x04, digToHEX(i));
    delay(500);
    disp.yellowLedOn();    
  }
}
