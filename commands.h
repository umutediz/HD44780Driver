#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "stdbool.h"
#include "stdint.h"

#define CFG_INTERFACE_POS (0)
#define CFG_BUS_WIDTH_POS (1)
#define CFG_LINE_COUNT_POS (2)
#define CFG_COL_COUNT_POS (3)

#define CFG_INTERFACE_MASK (0b00000001)
#define CFG_BUS_WIDTH_MASK (0b00000010)
#define CFG_LINE_COUNT_MASK (0b00000100)
#define CFG_COL_COUNT_MASK (0b11111000)  // Size is 5

#define PIN_COUNT (11)

#define LCD_DUMMY_PIN_PORT (0xAA)

#define MAX_ROW (4)
#define MAX_COL (0x80)

typedef enum { LCDFalse, LCDTrue } tLCDReturn;

typedef enum { LCD_BITS_4, LCD_BITS_8 } tBusWidth;

typedef enum {
  LCD_PARALLEL,
  LCD_I2C_PCF8574,
} tLCDInterface;

typedef struct {
  uint32_t _i2cHandler;
  uint32_t ports[PIN_COUNT];
  uint8_t* text[MAX_ROW];
  uint8_t pins[PIN_COUNT];
  uint8_t size[MAX_ROW];
  uint8_t _i2cAddr;
  uint16_t parallelPortMask : 11;
  uint8_t column : 6;
  uint8_t row : 2;
  uint8_t interface : 1;
  uint8_t busWidth : 1;
  uint8_t blink : 1;
  uint8_t enable : 1;
  uint8_t showCursor : 1;
  uint8_t backlight : 1;
} tLCD;

// Set I2C address if i2c used as interface.
tLCDReturn configI2C(tLCD* lcd, uint8_t addr);

// Configure ParallelPort for interfacing with display.
tLCDReturn configParallelPort(tLCD* lcd, uint32_t** ports, uint8_t* pins);

// Set bus width of the controller, select interface, configure display size in
// chars.
tLCDReturn LCDInit(tLCD* lcd, tLCDInterface interface, tBusWidth width,
                   uint8_t rowCount, uint8_t columnCount);

// Enable or disable display
tLCDReturn setDisplayState(tLCD* lcd, bool state);

// Enable or disable cursor blinking
tLCDReturn setBlinkState(tLCD* lcd, bool state);

// Hide or show cursor
tLCDReturn setShowCursor(tLCD* lcd, bool state);

// Clear display and return cursor to home.
tLCDReturn clearDisplay(tLCD* lcd);

// Positive integers shift display to right and negative integers shift left.
tLCDReturn shiftDisplay(tLCD* lcd, int8_t shiftCount);

// Set cursor position to home and reset shifting.
tLCDReturn returnHome(tLCD* lcd);

// Positive integers shift cursor to right and negative integers shift left.
tLCDReturn shiftCursor(tLCD* lcd, int8_t shiftCount);

// Set cursor position to given row and column
tLCDReturn setCursor(tLCD* lcd, uint8_t row, uint8_t column);

// Prepare LCD bufferto display later. Does not affect hardware until display
// function called.
tLCDReturn setText(tLCD* lcd, uint8_t row, uint8_t* text, uint8_t length);

// Display text on LCD
tLCDReturn display(tLCD* lcd);

// Low level write operation to LCD register
tLCDReturn writeReg(tLCD* lcd, uint16_t reg);

// Low level read operation from lcd register
tLCDReturn readReg(tLCD* lcd, uint16_t reg, uint8_t* data);
#endif