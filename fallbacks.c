#include "fallbacks.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define PIN_RS_POS (0)
#define PIN_RW_POS (1)
#define PIN_EN_POS (2)

void _LCDwait(uint32_t ms) {
	HAL_Delay(ms);
}

void _LCDwaitMicros(uint32_t micros) {
	HAL_Delay(micros / 1000 + 1);
}

tLCDReturn _i2cInit(uint32_t *i2c, uint8_t addr) {
	if (HAL_I2C_IsDeviceReady((I2C_HandleTypeDef*) i2c, addr, 10, 10) == HAL_OK) {
		return LCDTrue;
	}
	return LCDFalse;
}

tLCDReturn _i2cWrite(uint32_t *i2c, uint8_t addr, uint8_t reg) {
	uint8_t regWr[3] = { reg, reg | (1 << PIN_EN_POS), reg & ~(1 << PIN_EN_POS)};
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef*) i2c, addr, regWr, 3, 1);
	return LCDTrue;
}

tLCDReturn _i2cRead(uint32_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data) {
	(void) i2c;
	(void) addr;
	(void) reg;
	(void) data;
	return LCDTrue;
}
