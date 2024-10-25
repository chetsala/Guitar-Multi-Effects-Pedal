
#include "display_init.h"
#include "main.h"




void Display_Init(void)
{
    // Initialize the display hardware
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(0);

    // Clear the display with a default color
    BSP_LCD_Clear(LCD_COLOR_WHITE); // Clear the LCD display
    BSP_LCD_DisplayOn();
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24); //set the font of the text
}


void Display_Draw_Text(const char *text, uint16_t x, uint16_t y)
{

    BSP_LCD_DisplayStringAt(x, y, (uint8_t *)text, CENTER_MODE);
}