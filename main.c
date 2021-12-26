#include "commands.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

tLCD lcdObj;
tLCD* lcd = &lcdObj;

tLCD lcdObj2;
tLCD* lcd2 = &lcdObj2;

uint8_t text[MAX_COL];
uint8_t text2[MAX_COL];
uint8_t strachbook[MAX_COL];

uint8_t iter = 0;
uint8_t tmpsize;
int main() {
  // Configure I2C addresses for LCD's extension modules
  configI2C(lcd, 0x26);
  configI2C(lcd2, 0x27);

  // Initialize LCDs
  LCDInit(lcd, LCD_I2C_PCF8574, LCD_BITS_4, 2, 16);
  LCDInit(lcd2, LCD_I2C_PCF8574, LCD_BITS_4, 2, 16);

  // Reset LCDs
  // returnHome(lcd);
  // returnHome(lcd2);

  // Prepare text to display on the first row of displays
  tmpsize = sprintf(text, "%16s", "Hello World!!");
  setText(lcd, 0, text, tmpsize);
  setText(lcd2, 0, text, tmpsize);

  // Write to displays
  display(lcd);
  display(lcd2);

  while (1) {
    // Iterate through text until all characters printed.
    int16_t display1_limit = ((int16_t)(lcd->size[1]) - (int16_t)(lcd->column));
    int16_t display2_limit =
        ((int16_t)(lcd2->size[1]) - (int16_t)(lcd->column));
    for (uint8_t i = 0; i < MAX_COL; i++) {
      // Break for loop if both of texts fully displayed
      if ((i >= display1_limit) && (i >= display2_limit)) {
        returnHome(lcd);
        returnHome(lcd2);
        break;
      }
      if ((lcd->size[1] > lcd->column) && (i < display1_limit)) {
        shiftDisplay(lcd, -1);  // Shift display if text is too long to fit in
      } else {
        returnHome(lcd);  // Reset shifting
      }

      if ((lcd2->size[1] > lcd2->column) && (i < display2_limit)) {
        shiftDisplay(lcd2, -1);  // Shift display if text is too long to fit in
      } else {
        returnHome(lcd2);  // Reset shifting
      }

      usleep(400 * 1000);  // Wait for 200ms
    }

    // Prepare text to be displayed on lcd 1
    strcpy(strachbook, "This is a long string and needs to be shifted");
    tmpsize = sprintf(text2, "%-3d %s", iter, strachbook);
    setText(lcd, 1, text2, tmpsize);

    display(lcd);  // Display text on lcd1

    // Prepare text to be displayed on lcd2
    strcpy(strachbook, "Just a string");
    tmpsize = sprintf(text2, "%-13s %d", strachbook, 255 - iter);
    setText(lcd2, 1, text2, tmpsize);

    display(lcd2);  // Display text on lcd2
    iter++;
  }
}