/*
 ******************************************************************************
 * @file      ui_interface.c
 * @author    Chet Sala
 * @brief     UI Functioality
 ******************************************************************************
 */

#include "ui_interface.h"
#include "main.h"


void DrawScreen(int num) {
    char name[20];

    sprintf(name, "Media/%d.bmp", num);
    Storage_CheckBitmapFile(name, &uwBmplen);
    BSP_LCD_SelectLayer(1);
    Storage_OpenReadFile(uwInternelBuffer, name);
    BSP_LCD_DrawBitmap(0, 0, uwInternelBuffer);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_DrawBitmap(0, 0, uwInternelBuffer);
}

InitializeDemoScreen()
{

    /* LCD Initialization */
    DrawScreen(MENU_1);
}



void LCD_Config(void)
{
    /* LCD Initialization */
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));

    /* Enable the LCD */
    BSP_LCD_DisplayOn();

    /* Select the LCD Background Layer  */
    BSP_LCD_SelectLayer(1);

    /* Clear the Background Layer */
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* Select the LCD Foreground Layer  */
    BSP_LCD_SelectLayer(0);

    /* Clear the Foreground Layer */
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* Configure the transparency for foreground and background :
                     Increase the transparency */
    BSP_LCD_SetTransparency(0, 255);
    BSP_LCD_SetTransparency(1, 255);
}