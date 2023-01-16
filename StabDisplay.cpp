#include "StabDisplay.h"
#include <Arduino.h>
const uint8_t digitHEX[] = {
  0b11011110,
  0b00010010,
  0b10101110,
  0b10111010,
  0b01110010,
  0b11111000,
  0b11111100,
  0b10010010,
  0b11111110,
  0b11111010,
  0b00000000,
  0b01000000
};  //0~9, ,-

const uint8_t inputSegments[] = { 0x05, 0x03, 0x04 };
const uint8_t outputSegments[] = { 0x02, 0x01, 0x00 };

boolean DisplayOnInput = true;

StabDisplay::StabDisplay(uint8_t clk, uint8_t dio) {
  Clkpin = clk;
  Datapin = dio;
  pinMode(Clkpin, OUTPUT);
  pinMode(Datapin, OUTPUT);
}

uint8_t digToHEX(uint8_t digit) {
  return digitHEX[digit];
}

unsigned char reverse(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

int StabDisplay::writeByte(byte wr_data) {
  uint8_t i;
  for (i = 0; i < 8; i++)  //sent 8bit data
  {
    int pos = i;
    digitalWrite(Clkpin, LOW);
    if ((wr_data & (1 << pos)) != 0) {
      digitalWrite(Datapin, HIGH);  //LSB first
    } else {
      digitalWrite(Datapin, LOW);
    }
    //wr_data >>= 1;
    digitalWrite(Clkpin, HIGH);
  }
  digitalWrite(Clkpin, LOW);  //wait for the ACK
  digitalWrite(Datapin, HIGH);
  digitalWrite(Clkpin, HIGH);
  pinMode(Datapin, INPUT);

  delayMicroseconds(50);
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0) {
    pinMode(Datapin, OUTPUT);
    digitalWrite(Datapin, LOW);
  }
  delayMicroseconds(50);
  pinMode(Datapin, OUTPUT);
  delayMicroseconds(50);

  return ack;
}

//send start signal to StabDisplay
void StabDisplay::start(void) {
  digitalWrite(Clkpin, HIGH);  //send start signal to StabDisplay
  digitalWrite(Datapin, HIGH);
  digitalWrite(Datapin, LOW);
  digitalWrite(Clkpin, LOW);
}

//End of transmission
void StabDisplay::stop(void) {
  digitalWrite(Clkpin, LOW);
  digitalWrite(Datapin, LOW);
  digitalWrite(Clkpin, HIGH);
  digitalWrite(Datapin, HIGH);
}

// ************************** ФОРМИРУЕМ ДАННЫЕ *****************************
void StabDisplay::display(uint8_t DispData[]) {
  uint8_t SegData[3];
  for (byte i = 0; i < 3; i++) {
    lastData[i] = digitHEX[DispData[i]];
    SegData[i] = digitHEX[DispData[i]];
  }
  sendArray(SegData);
}

void StabDisplay::display(uint8_t BitAddr, uint8_t DispData) {
  uint8_t SegData;
  lastData[BitAddr] = digitHEX[DispData];
  SegData = digitHEX[DispData];
  sendByte(BitAddr, SegData);
}

void StabDisplay::displayByte(uint8_t DispData[]) {
  uint8_t SegData[3];
  for (byte i = 0; i < 3; i++) {
    lastData[i] = DispData[i];
    SegData[i] = DispData[i];
  }
  sendArray(SegData);
}

void StabDisplay::displayByte(uint8_t BitAddr, uint8_t DispData) {
  uint8_t SegData;
  lastData[BitAddr] = DispData;
  SegData = DispData + PointData;
  sendByte(BitAddr, SegData);
}

// ************************** ОТПРАВКА НА ДИСПЛЕЙ *****************************
void StabDisplay::sendByte(uint8_t BitAddr, int8_t sendData) {
  start();                    //start signal sent to StabDisplay from MCU
  writeByte(ADDR_FIXED);      //
  stop();                     //
  start();                    //
  writeByte(BitAddr | 0xc0);  //
  writeByte(sendData);        //
  stop();                     //
  start();                    //
  writeByte(Cmd_DispCtrl);    //
  stop();                     //
}

void StabDisplay::sendArray(uint8_t sendData[]) {
  start();                //start signal sent to StabDisplay from MCU
  writeByte(ADDR_FIXED);  //
  stop();                 //
  //writeByte(Cmd_SetAddr);  //
  for (byte i = 0; i < 3; i++) {
    start();  //
    writeByte((DisplayOnInput ? inputSegments[i] : outputSegments[i]) | 0xc0);
    writeByte(sendData[i]);  //
    stop();                  //
  }
  start();                  //
  writeByte(Cmd_DispCtrl);  //
  stop();                   //
}

// ******************************************
void StabDisplay::displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
  uint8_t dispArray[] = { bit0, bit1, bit2, bit3 };
  displayByte(dispArray);
}

void StabDisplay::display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
  uint8_t dispArray[] = { bit0, bit1, bit2, bit3 };
  display(dispArray);
}

void StabDisplay::clear(void) {
  display(0x00, 0x7f);
  display(0x01, 0x7f);
  display(0x02, 0x7f);
  display(0x03, 0x7f);
  display(0x04, 0x7f);
  display(0x05, 0x7f);
  lastData[0] = 0x00;
  lastData[1] = 0x00;
  lastData[2] = 0x00;
  lastData[3] = 0x00;
  lastData[4] = 0x00;
  lastData[5] = 0x00;
}

void StabDisplay::brightness(uint8_t brightness, uint8_t SetData, uint8_t SetAddr) {
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;  //Set the brightness and it takes effect the next time it displays.
}

void StabDisplay::point(boolean PointFlag, bool upd) {
  if (PointFlag) PointData = 0x80;
  else PointData = 0;
}

// ************************** ВСЯКИЕ ФУНКЦИИ *****************************
void StabDisplay::displayInputVoltage(int value) {
  displayVoltage(value);
}

void StabDisplay::displayOutputVoltage(int value) {
  displayVoltage(value, false);
}

void StabDisplay::displayVoltage(int value, bool input = true) {
  if (value > 999 || value < 0) {
    return;
  }
  if (input) {
    DisplayOnInput = true;
  } else {
    DisplayOnInput = false;
  }
  byte digits[3];
  value = abs(value);
  digits[0] = (int)(value / 100);  // количество сотен в числе
  uint16_t b = (int)(digits[0] * 100);
  digits[1] = (int)((value - b) / 10);  // получем десятки
  b += digits[1] * 10;                  // сумма сотен и десятков
  digits[2] = (int)(value - b);         // получаем количество единиц

  for (byte i = 0; i < 2; i++) {
    if (digits[i] == 0) {
      digits[i] = 10;
    } else {
      break;
    }
  }

  display(digits);
}

void swapBytes(byte* newByte, byte oldByte, byte newP, byte oldP) {
  byte newBit = 0;
  if (oldByte & (1 << oldP)) newBit = 1;
  else newBit = 0;
  *newByte = *newByte | (newBit << newP);
}

void StabDisplay::greenLedOn() {
  displayByte(0x00, 0x1);
}

void StabDisplay::yellowLedOn() {
  displayByte(0x03, 0x1);
}

void StabDisplay::redLedOn() {
  displayByte(0x02, 0x1);
}