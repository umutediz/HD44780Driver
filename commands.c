#include "commands.h"

#include "fallbacks.h"
#include "instructions.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define PIN_RS_POS (0)
#define PIN_RW_POS (1)
#define PIN_EN_POS (2)

#define __DATA_PIN_POS(X) (X + 3)

#define __GET_REG(X, POS, MASK) ((X & MASK) >> POS)

#define __CONVERT_TO_PCF(X) \
  ((X & 0xF8) | ((X & 0x100) >> 7) | ((X & 0x200) >> 9))

#ifdef RASPBERRYPI
#warning "Compiling for Raspberry Pi"
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

void _LCDwait(uint32_t ms) { usleep(ms * 1000); }
void _LCDwaitMicros(uint32_t micros) { usleep(micros); }

tLCDReturn _i2cInit(uint32_t* i2c, uint8_t addr) {
  wiringPiSetup();
  *i2c = wiringPiI2CSetup(addr);
  return LCDTrue;
}

tLCDReturn _i2cWrite(uint32_t* i2c, uint8_t addr, uint8_t reg) {
  (void)addr;
  wiringPiI2CWrite(*i2c, reg);
  wiringPiI2CWrite(*i2c, reg | (1 << PIN_EN_POS));
  _LCDwaitMicros(1);
  wiringPiI2CWrite(*i2c, reg & ~(1 << PIN_EN_POS));
  _LCDwaitMicros(1);
}

tLCDReturn _i2cRead(uint32_t* i2c, uint8_t addr, uint8_t reg, uint8_t* data) {
  *data = wiringPiI2CReadReg8(*i2c, reg);
  return LCDTrue;
}

#endif

// Low level write operation to LCD register
tLCDReturn writeReg(tLCD *lcd, uint16_t reg) {
	switch (lcd->interface) {
		case LCD_I2C_PCF8574:
			if (lcd->busWidth == LCD_BITS_4) {
				uint16_t tmpreg = reg | ((lcd->backlight) << 3);
				_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(tmpreg));
				tmpreg = (((reg & 0x0F) << 4) | (reg & 0x100) | (reg & 0x200)) | ((lcd->backlight) << 3);
				_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(tmpreg));
				return LCDTrue;
			}
			else {
				break;
			}

		case LCD_PARALLEL:
			break;
	}
	return LCDFalse;
}

// Low level read operation from lcd register
tLCDReturn readReg(tLCD *lcd, uint16_t reg, uint8_t *data) {
	switch (lcd->interface) {
		case LCD_I2C_PCF8574:
			return _i2cRead(lcd->_i2cHandler, lcd->_i2cAddr, reg, data);
		case LCD_PARALLEL:
			break;
	}
	return LCDFalse;
}
;

// Set I2C address if i2c used as interface.
tLCDReturn configI2C(tLCD *lcd, uint32_t *i2c, uint8_t addr) {
	lcd->_i2cHandler = i2c;
	lcd->_i2cAddr = addr;
	return _i2cInit(lcd->_i2cHandler, lcd->_i2cAddr);
}

// Configure ParallelPort for interfacing with display.
tLCDReturn configParallelPort(tLCD *lcd, uint32_t **ports, uint8_t *pins) {
	if (ports != 0 && pins != 0) {
		memcpy(lcd->ports, ports, sizeof(lcd->ports));
		memcpy(lcd->pins, pins, sizeof(lcd->pins));

		for (register uint8_t i = 0; i < sizeof(lcd->pins); i++) {
			if ((lcd->ports[i] == 0) || (lcd->pins[i] == 0)) {
				return LCDFalse;
			}
		}
		return LCDTrue;
	}
	else {
		return LCDFalse;
	}
}

// Set bus width of the controller, select interface, configure display size
// in chars.
tLCDReturn LCDInit(tLCD *lcd, tLCDInterface interface, tBusWidth width, uint8_t rowCount, uint8_t columnCount) {
	//
	tLCDReturn state = LCDTrue;

	if (rowCount <= MAX_ROW && columnCount <= MAX_COL) {
		lcd->busWidth = width;
		lcd->interface = interface;
		lcd->row = rowCount;
		lcd->column = columnCount;
		lcd->enable = true;
		lcd->showCursor = false;
		lcd->blink = false;
		lcd->backlight = true;
		memset(lcd->size, 0, sizeof(lcd->size));

		_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(generateFunctionSet(LCD_BITS_8, rowCount, FS_FONT_8)));
		_LCDwait(5);
		_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(generateFunctionSet(LCD_BITS_8, rowCount, FS_FONT_8)));
		_LCDwait(5);
		_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(generateFunctionSet(LCD_BITS_8, rowCount, FS_FONT_8)));
		_LCDwait(5);
		_i2cWrite(lcd->_i2cHandler, lcd->_i2cAddr, __CONVERT_TO_PCF(generateFunctionSet(LCD_BITS_4, rowCount, FS_FONT_8)));
		_LCDwait(5);

		state &= writeReg(lcd, generateEntryMode(true, false));  // Increment but do not shift
		_LCDwait(5);
		state &= writeReg(lcd, generateDisplayConfig(lcd->enable, lcd->showCursor, lcd->blink));
		_LCDwait(5);
		state &= writeReg(lcd, generateFunctionSet(lcd->busWidth, lcd->row, FS_FONT_8));

		state &= clearDisplay(lcd);
		_LCDwaitMicros(500);
		return state;

	}
	else {
		return LCDFalse;
	}
}

// Enable or disable display
tLCDReturn setDisplayState(tLCD *lcd, bool state) {
	lcd->enable = state;
	return writeReg(lcd, generateDisplayConfig(lcd->enable, lcd->showCursor, lcd->blink));
}

// Enable or disable cursor blinking
tLCDReturn setBlinkState(tLCD *lcd, bool state) {
	lcd->blink = state;
	return writeReg(lcd, generateDisplayConfig(lcd->enable, lcd->showCursor, lcd->blink));
}

// Hide or show cursor
tLCDReturn setShowCursor(tLCD *lcd, bool state) {
	lcd->showCursor = state;
	return writeReg(lcd, generateDisplayConfig(lcd->enable, lcd->showCursor, lcd->blink));
}

// Clear display and return cursor to home.
tLCDReturn clearDisplay(tLCD *lcd) {
	return writeReg(lcd, INS_CLEAR_DISP);
}

// Positive integers shift display to right and negative integers shift left.
tLCDReturn shiftDisplay(tLCD *lcd, int8_t shiftCount) {
	tLCDReturn state = LCDTrue;
	register uint8_t iter = 0;
	if (shiftCount == 0) {
		return LCDTrue;
	}
	else {
		while (iter++ < abs(shiftCount)) {
			state &= writeReg(lcd, generateDisplayShift(shiftCount > 0));
		}
	}
	return state;
}

tLCDReturn returnHome(tLCD *lcd) {
	tLCDReturn state = writeReg(lcd, INS_RETURN_HOME);
	_LCDwait(2);
	return state;
}

// Positive integers shift cursor to right and negative integers shift left.
tLCDReturn shiftCursor(tLCD *lcd, int8_t shiftCount) {
	if (shiftCount == 0) {
		return LCDTrue;
	}
	else {
		return writeReg(lcd, generateCursorMove(shiftCount > 0));
	}
}

// Set cursor position to given row and column
tLCDReturn setCursor(tLCD *lcd, uint8_t row, uint8_t column) {
	if (row < lcd->row && column < MAX_COL) {
		int rows[MAX_ROW] = { 0x00, 0x40, 0x14, 0x54 };
		return writeReg(lcd, generateSetDDRAMAddress(rows[row] + column));
	}
	else {
		return LCDFalse;
	}
}

/*
 * Populate driver's buffer with given text for desired row.
 * This function does not display text, only prepares it to be displayed when
 * desired.
 */
tLCDReturn setText(tLCD *lcd, uint8_t row, uint8_t *text, uint8_t length) {
	// Check given pointer is not NULL and size and row is in limits.
	if ((text != 0) && (length != 0) && (length <= (MAX_COL)) && (row <= lcd->row)) {
		// Make sure row argument can not cause overflow by masking out bits other
		// than LSB.
		lcd->text[row] = text;

		/*
		 * Make sure size of given text does not cause access violation in
		 * display's DDRAM space. Keeping size and pointer reduces memory cost and
		 * access times.
		 */
		lcd->size[row] = length <= MAX_COL ? length : MAX_COL;

		return LCDTrue;

	}
	else {
		return LCDFalse;
	}
}

// Display text on LCD
tLCDReturn display(tLCD *lcd) {
	tLCDReturn state = LCDTrue;
	uint8_t cl;
	for (register int8_t r = 0; r < lcd->row; r++) {
		if (lcd->size[r] > 0) {
			setCursor(lcd, r, 0);
			if (lcd->text[r] != 0) {
				cl = lcd->size[r] > lcd->column ? lcd->size[r] : lcd->column;
				for (register uint8_t c = 0; c < cl; c++) {
					if (c < lcd->size[r]) {
						state &= writeReg(lcd, INS_WRITE_MEM | lcd->text[r][c]);
					}
					else {
						state &= writeReg(lcd, INS_WRITE_MEM | 0x20);  // Print space
					}
				}
			}
		}
	}
	return state;
}
