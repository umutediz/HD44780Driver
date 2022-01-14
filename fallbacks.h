#include "commands.h"

__attribute__((weak)) void _LCDwait(uint32_t ms);
__attribute__((weak)) void _LCDwaitMicros(uint32_t micros);
__attribute__((weak)) tLCDReturn _i2cInit(uint32_t* i2c, uint8_t addr);
__attribute__((weak)) tLCDReturn _i2cWrite(uint32_t* i2c, uint8_t addr,
                                           uint8_t reg);
__attribute__((weak)) tLCDReturn _i2cRead(uint32_t* i2c, uint8_t addr,
                                          uint8_t reg, uint8_t* data);
