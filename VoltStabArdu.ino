#define CLK 2
#define DIO 3
#define RL1 10
#define RL2 11
#define RL3 12
#define RL4 15
#include <GyverNTC.h>
// термистор на пине А6
// сопротивление резистора 10к
// тепловой коэффициент 3950
GyverNTC therm(A6, 10000, 3950);

#include "StabDisplay.h"
StabDisplay disp(CLK, DIO);

uint32_t oneSecTimer, timer2, testTimer;
bool protectMode = false;
bool delayMode = false;
bool workingMode = false;
bool rotate = false;
uint16_t inputVoltage = 0, outputVoltage = 220, temperature, outputDispVal;

double sensorValue1 = 0;
double sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;

void setup() {
  all_off(); 
  Serial.begin(9600);
  disp.clear();
  disp.brightness(5);  // яркость, 0 - 7 (минимум - максимум)
  pinMode(RL1, OUTPUT);
  pinMode(RL2, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);
  test();
  all_off();
  countDown();
}

void loop() {
  temperature = round(therm.getTempAverage());
  inputVoltage = (int)measureVoltage();

  // При продакшне отключить!
  //  if (millis() - testTimer >= 1000) {
  //    testTimer = millis();
  //    inputVoltage += 10;
  //    if (inputVoltage > 290) {
  //      inputVoltage = 110;
  //    }
  //  }

  if (millis() - oneSecTimer >= 100) {
    oneSecTimer = millis();
    updateLedMode();
    updateInputDisplay();
    updateOutputDisplay();
  }

  if (millis() - timer2 >= 10000) {
    timer2 = millis();
    outputDispVal = rotate ? temperature : outputVoltage;
    rotate = !rotate;
  }
  tempCheck();
  voltageCheck();
}

void tempCheck() {
  if (temperature > 60) {
    protectMode = true;
    workingMode = false;
    delayMode = false;
    all_off();
    return;
  }
}

void voltageCheck() {

  if (inputVoltage < 140) {
    protectMode = true;
    workingMode = false;
    delayMode = false;
    all_off();
    return;
  }

  if (inputVoltage > 280) {
    protectMode = true;
    workingMode = false;
    delayMode = false;
    all_off();
    return;
  }

  if (inputVoltage >= 236) {
    RL1_on();
    RL2_on();
    RL3_on();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }

  if (inputVoltage >= 215) {
    RL1_on();
    RL2_on();
    RL3_off();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }

  if (inputVoltage >= 192) {
    RL1_on();
    RL2_off();
    RL3_on();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }

  if (inputVoltage >= 176) {
    RL1_on();
    RL2_off();
    RL3_off();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }

  if (inputVoltage >= 156) {
    RL1_off();
    RL2_off();
    RL3_on();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }

  if (inputVoltage >= 140) {
    RL1_off();
    RL2_off();
    RL3_off();
    RL4_on();
    protectMode = false;
    workingMode = true;
    delayMode = false;
    return;
  }
}

void updateInputDisplay() {
  disp.displayInputVoltage(inputVoltage);
}
void updateOutputDisplay() {
  disp.displayOutputVoltage(outputDispVal);
}

void updateLedMode() {
  if (protectMode) {
    disp.redLedOn();
    disp.yellowLedOff();
    disp.greenLedOff();
  }
  if (delayMode) {
    disp.redLedOff();
    disp.yellowLedOn();
    disp.greenLedOff();
  }
  if (workingMode) {
    disp.redLedOff();
    disp.yellowLedOff();
    disp.greenLedOn();
  }
}

void test() {
  delay(1000);
  disp.clear();
  disp.displayByte(0x04, 0b00100000);
  delay(10);
  RL1_on();
  delay(1000);
  RL1_off();
  delay(200);
  disp.displayByte(0x04, 0b00010000);
  delay(10);
  RL2_on();
  delay(1000);
  RL2_off();
  delay(200);
  disp.displayByte(0x04, 0b00001000);
  delay(10);
  RL3_on();
  delay(1000);
  RL3_off();
  delay(200);
  disp.displayByte(0x04, 0b00000100);
  delay(10);

  // YES
  disp.displayByte(0x05, 0b01111010);
  disp.displayByte(0x03, 0b11101100);
  disp.displayByte(0x04, 0b11111000);
  delay(1000);
}

void countDown() {
  disp.clear();
  disp.yellowLedOn();
  for (int i = 5; i >= 0; i--) {
    disp.displayInputVoltage(i);
    delay(1000);
  }
  disp.yellowLedOff();
}

void all_off() {
  RL1_off();
  RL2_off();
  RL3_off();
  RL4_off();
}
void RL1_on() {
  digitalWrite(RL1, LOW);
}
void RL1_off() {
  digitalWrite(RL1, HIGH);
}
void RL2_on() {
  digitalWrite(RL2, LOW);
}
void RL2_off() {
  digitalWrite(RL2, HIGH);
}
void RL3_on() {
  digitalWrite(RL3, LOW);
}
void RL3_off() {
  digitalWrite(RL3, HIGH);
}
void RL4_on() {
  digitalWrite(RL4, LOW);
}
void RL4_off() {
  digitalWrite(RL4, HIGH);
}

float measureVoltage() {
  for ( int i = 0; i < 100; i++ ) {
    sensorValue1 = analogRead(A0);
    if (analogRead(A0) > 511) {
      val[i] = sensorValue1;
    }
    else {
      val[i] = 0;
    }
    delay(1);
  }

  max_v = 0;

  for ( int i = 0; i < 100; i++ )
  {
    if ( val[i] > max_v )
    {
      max_v = val[i];
    }
    val[i] = 0;
  }
  if (max_v != 0) {
    VmaxD = max_v;
    VeffD = VmaxD / sqrt(2);
    Veff = (((VeffD - 420.76) / -90.24) * -210.2) + 210.2;
  }
  else {
    Veff = 0;
  }

  VmaxD = 0;
  return Veff;
}
