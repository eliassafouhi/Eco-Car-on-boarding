/*
 * ssd1306.h - minimal SSD1306 128x64 I2C OLED driver (framebuffer + 5x7 text)
 */
#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  64
#define SSD1306_ADDR    (0x3C << 1)   /* most 0.96" modules; some use 0x3D */

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c);
void SSD1306_Clear(void);
void SSD1306_Pixel(int x, int y, uint8_t on);
void SSD1306_Rect(int x, int y, int w, int h, uint8_t fill);
void SSD1306_Text(int x, int y, const char *s, uint8_t scale);
HAL_StatusTypeDef SSD1306_Update(void);
uint8_t SSD1306_Present(void);

#endif
