#ifndef _StabDisplay_h
#define _StabDisplay_h
#include <inttypes.h>
#include <Arduino.h>
class StabDisplay {
public:

  StabDisplay(uint8_t clk, uint8_t dio);  // объявление и инициализация

  void display(uint8_t DispData[]);                                      // выводит цифры массивом по ячейкам. От 0 до 9 (byte values[] = {3, 5, 9, 0}; )
  void display(uint8_t BitAddr, uint8_t DispData);                       // выводит цифру DispData в указанную ячейку дисплея BitAddr
  void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);  // если лень создавать массив, выводит цифры в ячейки

  void displayByte(uint8_t DispData[]);                                      // выводит байт вида 0xe6 и буквы-константы вида _a , _b .... массивом
  void displayByte(uint8_t BitAddr, uint8_t DispData);                       // выводит байт вида 0xe6 и буквы-константы вида _a , _b .... в ячейку
  void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);  // если лень создавать массив, выводит байты в ячейки

  void displayInputVoltage(int value);  // выводит число от 0 до 999
  void displayOutputVoltage(int value);  // выводит число от 0 до 999
  void displayVoltage(int value, bool input = true);  // выводит число от 0 до 999

  void clear(void);  // очистить дисплей

  void point(boolean PointFlag, bool upd = true);  // вкл / выкл точку (POINT_ON / POINT_OFF)

  void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);  // яркость 0 - 7
  
  void greenLedOn();

  void yellowLedOn();
  
  void redLedOn();

  void greenLedOff();

  void yellowLedOff();
  
  void redLedOff();
  
private:
  uint8_t lastData[6];
  void update();
  int writeByte(uint8_t wr_data);
  void start(void);
  void stop(void);

  void sendByte(uint8_t BitAddr, int8_t sendData);
  void sendArray(uint8_t sendData[]);
  void sendArray(uint8_t sendData[], uint8_t segments[]);

  uint8_t Cmd_SetData;
  uint8_t Cmd_SetAddr;
  uint8_t Cmd_DispCtrl;
  uint8_t PointData;

  uint8_t Clkpin;
  uint8_t Datapin;
  
};

uint8_t digToHEX(uint8_t digit);  // вернёт код цифры для displayByte
void swapBytes(byte* newByte, byte oldByte, byte oldP, byte newP);

//************definitions for TM1637*********************
#define ADDR_AUTO 0x40
#define ADDR_FIXED 0x44
#define STARTADDR 0xc0

/**** definitions for the clock point of the digit tube *******/
#define POINT_ON 0
#define POINT_OFF 1

/**************definitions for brightness***********************/
#define BRIGHT_DARKEST 0
#define BRIGHT_TYPICAL 2
#define BRIGHTEST 7

/************** БУКВЫ И СИМВОЛЫ *****************/

// #ifndef AVR
// #define _A 0x77
// #define _B 0x7f
// #define _C 0x39
// #define _D 0x3f
// #define _E 0x79
// #define _F 0x71
// #define _G 0x3d
// #define _H 0x76
// #define _J 0x1e
// #define _L 0x38
// #define _N 0x37
// #define _O 0x3f
// #define _P 0x73
// #define _S 0x6d
// #define _U 0x3e
// #define _Y 0x6e
// #define _a 0x5f
// #define _b 0x7c
// #define _c 0x58
// #define _d 0x5e
// #define _e 0x7b
// #define _f 0x71
// #define _h 0x74
// #define _i 0x10
// #define _j 0x0e
// #define _l 0x06
// #define _n 0x54
// #define _o 0x5c
// #define _q 0x67
// #define _r 0x50
// #define _t 0x78
// #define _u 0x1c
// #define _y 0x6e
// #define _dash 0x40
// #define _under 0x08
// #define _equal 0x48
// #define _empty 0x00
// #define _degree 0x63

// #define _0 0x3f
// #define _1 0x06
// #define _2 0x5b
// #define _3 0x4f
// #define _4 0x66
// #define _5 0x6d
// #define _6 0x7d
// #define _7 0x07
// #define _8 0x7f
// #define _9 0x6f

// #else

// enum TM1637_letters {
//   _A = 0x77,
//   _B = 0x7f,
//   _C = 0x39,
//   _D = 0x3f,
//   _E = 0x79,
//   _F = 0x71,
//   _G = 0x3d,
//   _H = 0x76,
//   _J = 0x1e,
//   _L = 0x38,
//   _N = 0x37,
//   _O = 0x3f,
//   _P = 0x73,
//   _S = 0x6d,
//   _U = 0x3e,
//   _Y = 0x6e,
//   _a = 0x5f,
//   _b = 0x7c,
//   _c = 0x58,
//   _d = 0x5e,
//   _e = 0x7b,
//   _f = 0x71,
//   _h = 0x74,
//   _i = 0x10,
//   _j = 0x0e,
//   _l = 0x06,
//   _n = 0x54,
//   _o = 0x5c,
//   _q = 0x67,
//   _r = 0x50,
//   _t = 0x78,
//   _u = 0x1c,
//   _y = 0x6e,
//   _dash = 0x40,
//   _under = 0x08,
//   _equal = 0x48,
//   _empty = 0x00,
//   _degree = 0x63,

//   _0 = 0x3f,
//   _1 = 0x06,
//   _2 = 0x5b,
//   _3 = 0x4f,
//   _4 = 0x66,
//   _5 = 0x6d,
//   _6 = 0x7d,
//   _7 = 0x07,
//   _8 = 0x7f,
//   _9 = 0x6f,
// };
// #endif
#endif