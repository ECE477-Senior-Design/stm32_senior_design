#include "stm32f4xx_hal.h"
#include "lcd.h"

static void LCD_Select() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
}

void LCD_Unselect() {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

static void LCD_Reset() {
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);
}

static void LCD_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&LCD_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);

}

static void LCD_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while(buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&LCD_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);

        buff += chunk_size;
        buff_size -= chunk_size;
    }
}

static void LCD_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
	LCD_WriteCommand(0x2A); // CASET
    {
        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        LCD_WriteData(data, sizeof(data));
    }

    // row address set
    LCD_WriteCommand(0x2B); // RASET
    {
        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        LCD_WriteData(data, sizeof(data));
    }

    // write to RAM
    LCD_WriteCommand(0x2C); // RAMWR
}

void LCD_Init() {
	LCD_Select();
	LCD_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
	LCD_WriteCommand(0x01);
    HAL_Delay(1000);

    // POWER CONTROL A
    LCD_WriteCommand(0xCB);
    {
        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        LCD_WriteData(data, sizeof(data));
    }

    // POWER CONTROL B
    LCD_WriteCommand(0xCF);
    {
        uint8_t data[] = { 0x00, 0xC1, 0x30 };
        LCD_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    LCD_WriteCommand(0xE8);
    {
        uint8_t data[] = { 0x85, 0x00, 0x78 };
        LCD_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    LCD_WriteCommand(0xEA);
    {
        uint8_t data[] = { 0x00, 0x00 };
        LCD_WriteData(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    LCD_WriteCommand(0xED);
    {
        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
        LCD_WriteData(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    LCD_WriteCommand(0xF7);
    {
        uint8_t data[] = { 0x20 };
        LCD_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    LCD_WriteCommand(0xC0);
    {
        uint8_t data[] = { 0x23 };
        LCD_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    LCD_WriteCommand(0xC1);
    {
        uint8_t data[] = { 0x10 };
        LCD_WriteData(data, sizeof(data));
    }

    // VCM CONTROL
    LCD_WriteCommand(0xC5);
    {
        uint8_t data[] = { 0x3E, 0x28 };
        LCD_WriteData(data, sizeof(data));
    }

    // VCM CONTROL 2
    LCD_WriteCommand(0xC7);
    {
        uint8_t data[] = { 0x86 };
        LCD_WriteData(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    LCD_WriteCommand(0x36);
    {
        uint8_t data[] = { 0x48 };
        LCD_WriteData(data, sizeof(data));
    }

    // PIXEL FORMAT
    LCD_WriteCommand(0x3A);
    {
        uint8_t data[] = { 0x55 };
        LCD_WriteData(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    LCD_WriteCommand(0xB1);
    {
        uint8_t data[] = { 0x00, 0x18 };
        LCD_WriteData(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    LCD_WriteCommand(0xB6);
    {
        uint8_t data[] = { 0x08, 0x82, 0x27 };
        LCD_WriteData(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    LCD_WriteCommand(0xF2);
    {
        uint8_t data[] = { 0x00 };
        LCD_WriteData(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    LCD_WriteCommand(0x26);
    {
        uint8_t data[] = { 0x01 };
        LCD_WriteData(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    LCD_WriteCommand(0xE0);
    {
        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        LCD_WriteData(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    LCD_WriteCommand(0xE1);
    {
        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        LCD_WriteData(data, sizeof(data));
    }

    // EXIT SLEEP
    LCD_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    LCD_WriteCommand(0x29);

    // MADCTL
    LCD_WriteCommand(0x36);
    {
        uint8_t data[] = { LCD_ROTATION };
        LCD_WriteData(data, sizeof(data));
    }

    LCD_Unselect();
}

void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
        return;

    LCD_Select();

    LCD_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    LCD_WriteData(data, sizeof(data));

    LCD_Unselect();
}

static void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    LCD_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                LCD_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                LCD_WriteData(data, sizeof(data));
            }
        }
    }
}

void LCD_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
	LCD_Select();

    while(*str) {
        if(x + font.width >= LCD_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= LCD_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        LCD_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    LCD_Unselect();
}

void LCD_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return;
    if((x + w - 1) >= LCD_WIDTH) w = LCD_WIDTH - x;
    if((y + h - 1) >= LCD_HEIGHT) h = LCD_HEIGHT - y;

    LCD_Select();
    LCD_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            HAL_SPI_Transmit(&LCD_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);

        }
    }

    LCD_Unselect();
}

void LCD_FillScreen(uint16_t color) {
	LCD_FillRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return;
    if((x + w - 1) >= LCD_WIDTH) return;
    if((y + h - 1) >= LCD_HEIGHT) return;

    LCD_Select();
    LCD_SetAddressWindow(x, y, x+w-1, y+h-1);
    LCD_WriteData((uint8_t*)data, sizeof(uint16_t)*w*h);
    LCD_Unselect();
}

void LCD_InvertColors(bool invert) {
	LCD_Select();
	LCD_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
	LCD_Unselect();
}

//Custom Functions
void LCD_Test(void) {
	LCD_FillScreen(LCD_BLACK);
	for (int x = 0; x < LCD_WIDTH; x++) {
		LCD_DrawPixel(x, 0, LCD_RED);
		LCD_DrawPixel(x, LCD_HEIGHT-1, LCD_RED);
	}
	for (int y = 0; y < LCD_HEIGHT; y++) {
		LCD_DrawPixel(0, y, LCD_RED);
		LCD_DrawPixel(LCD_WIDTH-1, y, LCD_RED);
	}
	HAL_Delay(1000);
}
