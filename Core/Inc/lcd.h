//GPIO Pins
//PA15: GPIO_OUTPUT | PB3: SPI1_SCK | PB4: GPIO_OUTPUT | PB5: SPI1_MOSI | PB8: GPIO_OUTPUT
//SPI Settings
//Mode: Transmit Only Master
//Hardware NSS Signal: Disable
//Baud Rate: 2.25 M Bit/s

#ifndef LCD_H_
#define LCD_H_

#include "fonts.h"
#include <stdbool.h>

#define LCD_MADCTL_MY  0x80
#define LCD_MADCTL_MX  0x40
#define LCD_MADCTL_MV  0x20
#define LCD_MADCTL_ML  0x10
#define LCD_MADCTL_RGB 0x00
#define LCD_MADCTL_BGR 0x08
#define LCD_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define LCD_SPI_PORT hspi1
extern SPI_HandleTypeDef LCD_SPI_PORT;

#define LCD_RES_Pin       GPIO_PIN_15
#define LCD_RES_GPIO_Port GPIOA
#define LCD_CS_Pin        GPIO_PIN_8
#define LCD_CS_GPIO_Port  GPIOB
#define LCD_DC_Pin        GPIO_PIN_4
#define LCD_DC_GPIO_Port  GPIOB

// default orientation
//#define LCD_WIDTH  240
//#define LCD_HEIGHT 320
//#define LCD_ROTATION (LCD_MADCTL_MX | LCD_MADCTL_BGR)

// rotate right
/*
#define LCD_WIDTH  320
#define LCD_HEIGHT 240
#define LCD_ROTATION (LCD_MADCTL_MX | LCD_MADCTL_MY | LCD_MADCTL_MV | LCD_MADCTL_BGR)
*/

// rotate left
/*
#define LCD_WIDTH  320
#define LCD_HEIGHT 240
#define LCD_ROTATION (LCD_MADCTL_MV | LCD_MADCTL_BGR)
*/

// upside down

#define LCD_WIDTH  240
#define LCD_HEIGHT 320
#define LCD_ROTATION (LCD_MADCTL_MY | LCD_MADCTL_BGR)


/****************************/

// Color definitions
#define LCD_GRAY    0x7BEF
#define LCD_BLACK   0x0000
#define LCD_BLUE    0x001F
#define LCD_RED     0xF800
#define LCD_GREEN   0x07E0
#define LCD_CYAN    0x07FF
#define LCD_MAGENTA 0xF81F
#define LCD_YELLOW  0xFFE0
#define LCD_WHITE   0xFFFF
#define LCD_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

//Font Definitions
//#define FONT Font_7x10
#define FONT Font_11x18
//#define FONT Font_16x26

// call before initializing any SPI devices
void LCD_Unselect(void);

void LCD_Init(void);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_WriteStringCentered(uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_FillScreen(uint16_t color);
void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void LCD_InvertColors(bool invert);

//Custom Functions
void LCD_Test(void);

#endif /* LCD_H_ */
