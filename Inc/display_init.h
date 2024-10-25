#ifndef INC_DISPLAY_INIT_H_
#define INC_DISPLAY_INIT_H_


/*
 * Includes
 */

#include "stm32746g_discovery_lcd.h"

#include "rk043fn48h.h" // Display controller header









/*
 * Function Prototypes
 */

void Display_Init(void);

void Display_Draw_Text(const char *text, uint16_t x, uint16_t y);




#endif /* INC_DISPLAY_INIT_H_ */