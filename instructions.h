#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "stdbool.h"
#include "stdint.h"

#define INS_CLEAR_DISP (0x01)

#define INS_RETURN_HOME (0x02)

#define INS_ENTRY_MODE (0x04)
#define EM_INC_DEC_MASK (1 << 1)
#define EM_DISP_SHIFT_MASK (1 << 0)
#define EM_DECREMENT (0)
#define EM_INCREMENT (1)

#define INS_DISPLAY_CONFIG (0x08)
#define DC_DISPLAY_STATE_MASK (1 << 2)
#define DC_CURSOR_STATE_MASK (1 << 1)
#define DC_CURSOR_BLINK_MASK (1 << 0)

#define INS_CURSOR_DISPLAY_SHIFT (0x10)
#define CDS_SC_MASK (1 << 3)
#define CDS_RL_MASK (1 << 2)
#define CDS_ROTATE_LEFT (0 << 2)
#define CDS_ROTATE_RIGHT (1 << 2)
#define CDS_MOVE_CURSOR (0 << 3)
#define CDS_SHIFT_DISPLAY (1 << 3)

#define INS_FUNCTION_SET (0x20)
#define FS_BUS_WIDTH_MASK (1 << 4)
#define FS_NUMBER_OF_LINES_MASK (1 << 3)
#define FS_FONT_MASK (1 << 2)
#define FS_BW_4BIT (0)
#define FS_BW_8BIT (1)
#define FS_SINGLE_LINE (0)
#define FS_DOUBLE_LINE (1)
#define FS_FONT_8 (0)
#define FS_FONT_10 (1)

#define INS_SET_CGRAM_ADDR (0x40)  // From 0x40 to 0x7F, 6bit address

#define INS_SET_DDRAM_ADDR (0x80)  // From 0x80 to 0xFF, 7bit address

#define INS_IS_BUSY (0x100)
#define BUSY_FLAG_MASK (1 << 7)
#define BUSY_ADDRESS_MASK (~(BUSY_FLAG_MASK))

#define INS_WRITE_MEM (0x200)  // From 0x200 to 0x2FF, 8 bit data
#define INS_REG_MEM (0x300)    // From 0x300 to 0x3FF, 8 bit data

uint8_t generateEntryMode(bool cursorMove, bool dispShift);
uint8_t generateDisplayConfig(bool disp, bool cursor, bool blink);
uint8_t generateFunctionSet(bool width, bool lineCount, bool font);
uint8_t generateDisplayShift(bool direction);
uint8_t generateCursorMove(bool direction);
uint8_t generateSetCGRAMAddress(uint8_t addr);
uint8_t generateSetDDRAMAddress(uint8_t addr);

#endif