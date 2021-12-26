#include "fallbacks.h"

#include "stdio.h"

void _wait(uint32_t ms) {
  while (ms--)
    ;
}

void _waitMicros(uint32_t micros) {
  micros = micros / 1000;
  while (micros--)
    ;
}

tLCDReturn _i2cInit(uint32_t* i2c, uint8_t addr) {
  (void)i2c;
  // printf("Init -> 0x%X\n", addr);
  return LCDTrue;
}

tLCDReturn _i2cWrite(uint32_t* i2c, uint8_t addr, uint8_t reg) {
  (void)i2c;
  printf("0x%X\n", reg);
  printf("0x%X\n", reg | (1 << 2));
  printf("0x%X\n", reg & ~(1 << 2));
  return LCDTrue;
}

tLCDReturn _i2cRead(uint32_t* i2c, uint8_t addr, uint8_t reg, uint8_t* data) {
  (void)i2c;
  (void)addr;
  (void)reg;
  (void)data;
  return LCDTrue;
}