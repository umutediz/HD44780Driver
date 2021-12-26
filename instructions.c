#include "instructions.h"

uint8_t generateEntryMode(bool cursorMove, bool dispShift) {
  return INS_ENTRY_MODE | (cursorMove ? EM_INC_DEC_MASK : 0) |
         (dispShift ? EM_DISP_SHIFT_MASK : 0);
}

uint8_t generateDisplayConfig(bool disp, bool cursor, bool blink) {
  return INS_DISPLAY_CONFIG | (disp ? DC_DISPLAY_STATE_MASK : 0) |
         (cursor ? DC_CURSOR_STATE_MASK : 0) |
         (blink ? DC_CURSOR_BLINK_MASK : 0);
}

uint8_t generateFunctionSet(bool width, bool lineCount, bool font) {
  // 5x10 font is only available with single line displays.
  return INS_FUNCTION_SET | (width ? FS_BUS_WIDTH_MASK : 0) |
         (lineCount ? FS_NUMBER_OF_LINES_MASK : 0) |
         ((font && !lineCount) ? FS_FONT_MASK : 0);
}

uint8_t generateDisplayShift(bool direction) {
  return (INS_CURSOR_DISPLAY_SHIFT |
          (direction ? CDS_ROTATE_RIGHT : CDS_ROTATE_LEFT) | CDS_SHIFT_DISPLAY);
}

uint8_t generateCursorMove(bool direction) {
  return (INS_CURSOR_DISPLAY_SHIFT |
          (direction ? CDS_ROTATE_RIGHT : CDS_ROTATE_LEFT) | CDS_MOVE_CURSOR);
}

uint8_t generateSetCGRAMAddress(uint8_t addr) {
  return (INS_SET_CGRAM_ADDR | (addr & 0x3F));
}

uint8_t generateSetDDRAMAddress(uint8_t addr) {
  return (INS_SET_DDRAM_ADDR | (addr & 0x7F));
}
