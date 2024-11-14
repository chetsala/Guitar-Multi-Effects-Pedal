/*
 ******************************************************************************
 * @file      ui_interface.c
 * @author    Chet Sala
 * @brief     UI Functioality
 ******************************************************************************
 */

#include "ui_interface.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "gui/gui.h"
#include "ex03/macros.h"
#include "string.h"
#include "gui/leds.h"
#include "gui/whaondas.h"
#include "gui/cartel.h"
#include "prototypes.h"
#include "gui/tremoloondas.h"
#include "gui/vibratoondas.h"




#define DELAY          0
#define OCTAVADOR      1
#define CHORUS         2
#define PHASER         3
#define WAH            4
#define REVERB         5
#define DISTORTION     6
#define FLANGER        7
#define TREMOLO        8
#define VIBRATO        9
#define EQ             10
#define RINGMOD        11
#define RECORD         12
#define LEFT           0
#define MENU_1         14
#define MENU_2         15
#define MENU_3         16
#define RIGHT          1
#define HOME           2
#define LEFT_MENU      17
#define RIGHT_MENU     18

#define KNOB           0
#define SLIDER         1


extern PedalElement *pedal_effects[13];
static uint32_t color_mode_init = 0;

DMA2D_HandleTypeDef hDma2dHandler;

// externally defined structures & functions

LTDC_HandleTypeDef hLtdcHandler;

extern int selection_pedal, seleccion_menu;
extern int pedal_individual;
extern LinkElementMenu *Left_Menu_Arrow, *Right_Menu_Arrow;

FIL perillas_ptr;
static uint8_t *Knob_Buffer_35x35x25 = (uint8_t *)(KNOB_35x35x25);
static uint8_t *Knob_Buffer_42x41x25 = (uint8_t *)(KNOB_42x41x25);
static uint8_t *Knob_Buffer_52x52x25 = (uint8_t *)(KNOB_52x52x25);
static uint8_t *Knob_Buffer_eq_slider = (uint8_t *)(KNOB_EQ_SLIDER);

//Variables Knobs Spritesheet_slider
static SpriteSheet knob3535;// = { .pixels = Perillas_Buffer_35x35x25,
//.spriteWidth = 35, .spriteHeight = 35, .numSprites = 25, .format =
//CM_ARGB8888};
static SpriteSheet knob4241; //= { .pixels = Perillas_Buffer_42x41x25,
//.spriteWidth = 42, .spriteHeight = 41, .numSprites = 25, .format =
//CM_ARGB8888};
static SpriteSheet knob5252;// = { .pixels = perilla52x52x25,
//.spriteWidth = 52, .spriteHeight = 52, .numSprites = 25, .format =
//CM_ARGB8888};//CM_RGB888
static SpriteSheet eq_slider; //= { .pixels = eq_slider_30,
//.spriteWidth = 30, .spriteHeight = 83, .numSprites = 26, .format =
//CM_ARGB8888};//CM_RGB888
static SpriteSheet whaonda = { .pixels      = whaondas,
                               .spriteWidth = 41,      .spriteHeight= 41, .numSprites = 6, .format =
                                   CM_ARGB8888 };//CM_RGB888
static SpriteSheet vibratoonda = { .pixels      = vibratoondas,
                                   .spriteWidth = 35,          .spriteHeight= 35, .numSprites = 4, .format =
                                       CM_ARGB8888 };//CM_RGB888
static SpriteSheet tremoloonda = { .pixels      = tremoloondas,
                                   .spriteWidth = 41,          .spriteHeight= 41, .numSprites = 4, .format =
                                       CM_ARGB8888 };//CM_RGB888

static uint8_t buffer_images[196000] = { 0 }; //10816
static uint8_t colorModeStrides[5] = { 4, 3, 2, 2, 2 };

void read_files_knobs(void) {
    f_open(&knobs_ptr, "/Perillas/35.bin", FA_READ);
    f_read(&knobs_ptr, Knob_Buffer_35x35x25, 122500, NULL);
    f_close(&knobs_ptr);

    f_open(&knobs_ptr, "/Perillas/42.bin", FA_READ);
    f_read(&knobs_ptr, Knob_Buffer_42x41x25, 172200, NULL);
    f_close(&knobs_ptr);

    f_open(&knobs_ptr, "/Perillas/52.bin", FA_READ);
    f_read(&knobs_ptr, Knob_Buffer_52x52x25, 270400, NULL);
    f_close(&knobs_ptr);

    f_open(&knobs_ptr, "/Perillas/eq.bin", FA_READ);
    f_read(&knobs_ptr, Knob_Buffer_eq_slider, 258960, NULL);
    f_close(&knobs_ptr);
}

void init_LL_ConvertLine_DMA2D(uint32_t color_mode) {
    HAL_DMA2D_DeInit(&hDma2dHandler);

    hDma2dHandler.Init.Mode = DMA2D_M2M_PFC;
    hDma2dHandler.Init.ColorMode = DMA2D_ARGB8888;
    hDma2dHandler.Init.OutputOffset = 0;

    hDma2dHandler.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;                 //
    hDma2dHandler.LayerCfg[1].InputAlpha = 0xFF;                //FF
    hDma2dHandler.LayerCfg[1].InputColorMode = color_mode;
    hDma2dHandler.LayerCfg[1].InputOffset = 0;
    hDma2dHandler.Instance = DMA2D;

    color_mode_init = color_mode;

    if (HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK) {
        if (HAL_DMA2D_ConfigLayer(&hDma2dHandler, 1) == HAL_OK) {
        }
    }
}

void LL_ConvertLineToARGB8888(void *src, void *dst, uint32_t xstride, uint32_t color_mode) {
    if (color_mode != color_mode_init) {
        init_LL_ConvertLine_DMA2D(color_mode);
    }

    if (HAL_DMA2D_Start(&hDma2dHandler, (uint32_t)src, (uint32_t)dst, xstride,
                        1) == HAL_OK) {
        HAL_DMA2D_PollForTransfer(&hDma2dHandler, 10);
    }
}

// GUI element constructors
GUIElement * guiElement(uint8_t id, char *label, uint16_t x, uint16_t y,
                        SpriteSheet *sprite, GUICallback cb) {
    GUIElement *e = (GUIElement *)calloc(1, sizeof(GUIElement));

    e->id = id;
    e->x = x;
    e->y = y;
    e->width = sprite->spriteWidth;
    e->height = sprite->spriteHeight;
    e->sprite = sprite;
    e->label = label;
    e->callback = cb;
    e->state = GUI_OFF | GUI_DIRTY;
    return e;
}

// push button functions


// dial button functions

void handleDialButton(GUIElement *bt, GUITouchState *touch) {
    int16_t delta = 0;
    float newVal = 0;

    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];
        DialButtonState *db = (DialButtonState *)bt->userData;

        if (bt->state == GUI_HOVER) {
            if (db->orientacion == KNOB) {
                delta = (x - db->startX);
                newVal = db->startValue + db->sensitivity * delta;
                db->value = CLAMP(newVal, 0.0f, 1.0f);
            } else if (db->orientacion == SLIDER) {
                delta = (y - db->startY);
                newVal = db->startValue + (db->sensitivity / 2) * delta;
                db->value = CLAMP(newVal, 0.0f, 1.0f);
            }

            bt->state = GUI_DIRTY | GUI_HOVER;

            if (bt->callback != NULL) {
                bt->callback(bt);
            }
        } else if (x >= bt->x && x < bt->x + bt->width && y >= bt->y && y < bt->y + bt->height) {
            bt->state = GUI_HOVER;
            db->startX = x;
            db->startY = y;
            db->startValue = db->value;
        }
    } else if (bt->state == GUI_HOVER) {
        bt->state = GUI_OFF | GUI_DIRTY;
    }
}

void renderDialButton(GUIElement *bt) {
    uint8_t id = 0;

    if (bt->state & GUI_DIRTY) {
        SpriteSheet *sprite = bt->sprite;
        DialButtonState *db = (DialButtonState *)bt->userData;

        if (db->orientacion == SLIDER) {
            if (pedal_effects[10]->push->push_state == GUI_ON || pedal_effects[10]->push->push_state == (GUI_OFF | GUI_HOVER)) {
                id = (uint8_t)(db->value * (float32_t)(12));
            } else if (pedal_effects[10]->push->push_state == GUI_OFF || pedal_effects[10]->push->push_state == (GUI_ON | GUI_HOVER)) {
                id = (uint8_t)13 + (db->value * (float32_t)(12));
            }
        } else {
            id = (uint8_t)(db->value * (float)(sprite->numSprites - 1));
        }

        drawSprite(bt->x, bt->y, id, sprite);
        bt->state &= ~((uint16_t)GUI_DIRTY);
    }
}

void drawSprite(uint16_t x, uint16_t y, uint8_t id, SpriteSheet *sprite) {
    uint32_t lcdWidth = BSP_LCD_GetXSize();
    uint32_t address = hLtdcHandler.LayerCfg[LTDC_ACTIVE_LAYER].FBStartAdress
        + (((lcdWidth * y) + x) << 2);
    uint16_t width = sprite->spriteWidth;
    uint16_t height = sprite->spriteHeight;
    uint32_t stride = colorModeStrides[sprite->format];

    memcpy(buffer_images, (uint8_t *)&sprite->pixels[(id * width * height) * stride], width * height * stride);

    uint8_t *pixels = buffer_images;

    stride *= width;
    lcdWidth <<= 2;

    while (--height) {
        LL_ConvertLineToARGB8888(pixels, (uint32_t *)address, width, CM_ARGB8888);
        address += lcdWidth;
        pixels += stride;
    }
}

void drawBitmapRaw(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                   uint8_t *pixels, uint32_t colorMode, uint8_t active_layer) {
    uint32_t lcdWidth = BSP_LCD_GetXSize();
    uint32_t address = hLtdcHandler.LayerCfg[active_layer].FBStartAdress
        + (((lcdWidth * y) + x) << 2);                //LTDC_ACTIVE_LAYER
    uint16_t stride = width * colorModeStrides[colorMode];

    lcdWidth <<= 2;

    while (--height) {
        LL_ConvertLineToARGB8888(pixels, (uint32_t *)address, width, colorMode);
        address += lcdWidth;
        pixels += stride;
    }
}

void guiForceRedraw(GUI *gui) {
    for (uint8_t i = 0; i < gui->numItems; i++) {
        gui->items[i]->state |= GUI_DIRTY;
    }
}

void guiUpdate(KnobElement *gui, GUITouchState *touch) {
    for (uint8_t i = 0; i < gui->numItems; i++) {
        GUIElement *e = gui->dials[i];
        e->handler(e, touch);
        e->render(e);
    }
}

void guiUpdateTouch(TS_StateTypeDef *raw, GUITouchState *touch) {
    touch->touchDetected = raw->touchDetected;
    touch->touchX[0] = raw->touchX[0];
    touch->touchY[0] = raw->touchY[0];
}


void linkRequestHandlers_pedal_individual(PedalElement *bt, GUITouchState *touch) {
    bt->buttons->rightArrow->handler(bt, touch);
    bt->buttons->leftArrow->handler(bt, touch);
    bt->buttons->home->handler(bt, touch);
}

void linkRequestHandler_menu(PedalElement **bt, GUITouchState *touch) {
    if (selection_menu == MENU_1) {
        for (uint8_t i = 0; i < 6; i++) {
            bt[i]->link->handler(bt[i], touch);
        }
    } else if (selection_menu == MENU_2) {
        for (uint8_t i = 6; i < 12; i++) {
            bt[i]->link->handler(bt[i], touch);
        }
    } else if (selection_menu == MENU_3) {
    }
}

void linkRequestHandler_Flechas_Menu(LinkElementMenu *bt, GUITouchState *touch) {
    bt->handler(bt, touch);
}

void PushRequestHandler_menu(PedalElement **bt, GUITouchState *touch) {
    if (selection_menu == MENU_1) {
        for (uint8_t i = 0; i < 6; i++) {
            bt[i]->push->push_menu->handler(bt[i], touch);
        }
    } else if (selection_menu == MENU_2) {
        for (uint8_t i = 6; i < 12; i++) {
            bt[i]->push->push_menu->handler(bt[i], touch);
        }
    } else if (selection_menu == MENU_3 && bt[RECORD]->push->push_state != GUI_ON) {
        bt[RECORD]->push->push_menu->handler(bt[RECORD], touch);
    }
}

GUIElement * guiDialButton(uint8_t id, char *label, uint16_t x, uint16_t y,
                           float val, float sens, uint8_t orientation, SpriteSheet *sprite, GUICallback cb) {
    GUIElement *e = guiElement(id, label, x, y, sprite, cb);
    DialButtonState *db = (DialButtonState *)calloc(1, sizeof(DialButtonState));

    e->handler = handleDialButton;
    e->render = renderDialButton;
    e->userData = db;
    db->value = val;
    db->sensitivity = sens;
    db->orientation = orientation;
    return e;
}

LinkElement * initPushLink(uint8_t name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, GUICallbackLink cb, LinkHandler han) {
    LinkElement *e = calloc(1, sizeof(LinkElement));

    e->name = name;
    e->x = x;
    e->y = y;
    e->width = width;
    e->height = height;
    e->callback = cb;
    e->handler = han;
    return e;
}

LinkElementMenu * initPushLinkMenu(uint8_t name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, GUICallbackLinkMenu cb, LinkHandlerMenu han) {
    LinkElementMenu *e = calloc(1, sizeof(LinkElementMenu));

    e->name = name;
    e->x = x;
    e->y = y;
    e->width = width;
    e->height = height;
    e->callback = cb;
    e->handler = han;
    return e;
}

KnobElement * initKnob(uint8_t num) {
    KnobElement *gui = (KnobElement *)calloc(1, sizeof(KnobElement));

    gui->dials = (GUIElement **)calloc(num, sizeof(GUIElement *));
    gui->numItems = num;
    return gui;
}

void PedalForceRedraw(PedalElement *gui) {
    for (uint8_t i = 0; i < gui->knob->numItems; i++) {
        gui->knob->dials[i]->state |= GUI_DIRTY;
    }

    if (gui->push->push_state == GUI_ON || gui->push->push_state == GUI_OFF) {
        gui->push->push_state |= GUI_HOVER;
        gui->push->push_state ^= GUI_ONOFF_MASK;
    }
}

void MenuForceRedraw(void) {
    if (selection_menu == MENU_1) {
        for (int aux_i = 0; aux_i < 6; aux_i++) {
            if (PedalElement[aux_i]->push->push_state == GUI_ON || PedalElement[aux_i]->push->push_state == GUI_OFF) {
                PedalElement[aux_i]->push->push_state |= GUI_HOVER;
                PedalElement[aux_i]->push->push_state ^= GUI_ONOFF_MASK;
            }
        }
    } else if (selection_menu == MENU_2) {
        for (int aux_i = 6; aux_i < 12; aux_i++) {
            if (PedalElement[aux_i]->push->push_state == GUI_ON || PedalElement[aux_i]->push->push_state == GUI_OFF) {
                PedalElement[aux_i]->push->push_state |= GUI_HOVER;
                PedalElement[aux_i]->push->push_state ^= GUI_ONOFF_MASK;
            }
        }
    } else if (seleccion_menu == MENU_3) {
        if (PedalElement[RECORD]->push->push_state == GUI_ON || PedalElement[RECORD]->push->push_state == GUI_OFF) {
            PedalElement[RECORD]->push->push_state |= GUI_HOVER;
            PedalElement[RECORD]->push->push_state ^= GUI_ONOFF_MASK;
        }
    }
}

//Handles
void handlePushMenuButton(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->push->push_menu->x && x < bt->push->push_menu->x + bt->push->push_menu->width &&
            y >= bt->push->push_menu->y && y < bt->push->push_menu->y + bt->push->push_menu->height) {
            switch (bt->push->push_state) {
                case GUI_OFF:
                    bt->push->push_state |= GUI_HOVER | GUI_DIRTY;
                    break;

                case GUI_ON:
                    bt->push->push_state |= GUI_HOVER | GUI_DIRTY;
                    break;

                default:
                    break;
            }
        }
    } else if (bt->push->push_state & GUI_HOVER) {
        // clear hover flag
        bt->push->push_state &= ~((uint16_t)GUI_HOVER);
        // mark dirty (force redraw)
        bt->push->push_state |= GUI_DIRTY;
        // invert on/off bitmask
        bt->push->push_state ^= GUI_ONOFF_MASK;

        if (bt->push->push_menu->callback != NULL) {
            bt->push->push_menu->callback(bt);
        }
    }
}

void handlePushIndividualButton(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->push->push_indiv->x && x < bt->push->push_indiv->x + bt->push->push_indiv->width && y >= bt->push->push_indiv->y && y < bt->push->push_indiv->y + bt->push->push_indiv->height) {
            switch (bt->push->push_state) {
                case GUI_OFF:
                    bt->push->push_state |= GUI_HOVER;
                    break;

                case GUI_ON:
                    bt->push->push_state |= GUI_HOVER;
                    break;

                default:
                    break;
            }
        }
    } else if (bt->push->push_state & GUI_HOVER) {
        // clear hover flag
        bt->push->push_state &= ~((uint16_t)GUI_HOVER);
        // mark dirty (force redraw)
        bt->push->push_state |= GUI_DIRTY;
        // invert on/off bitmask
        bt->push->push_state ^= GUI_ONOFF_MASK;

        if (bt->push->push_indiv->callback != NULL) {
            bt->push->push_indiv->callback(bt);
        }
    }
}

void handleLinkButton(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->link->x && x < bt->link->x + bt->link->width && y >= bt->link->y && y < bt->link->y + bt->link->height) {
            bt->link->callback(bt);
        }
    }
}

void handleRightButtonPedal(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->buttons->rightArrow->x && x < bt->buttons->rightArrow->x + bt->buttons->rightArrow->width &&
            y >= bt->buttons->rightArrow->y && y < bt->buttons->rightArrow->y + bt->buttons->rightArrow->height) {
            bt->buttons->rightArrow->callback(bt);
        }
    }
}

void handleLeftButtonPedal(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->buttons->leftArrow->x && x < bt->buttons->leftArrow->x + bt->buttons->leftArrow->width &&
            y >= bt->buttons->leftArrow->y && y < bt->buttons->leftArrow->y + bt->buttons->leftArrow->height) {
            bt->buttons->leftArrow->callback(bt);
        }
    }
}

void handleHomeButtonPedal(PedalElement *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->buttons->home->x && x < bt->buttons->home->x + bt->buttons->home->width &&
            y >= bt->buttons->home->y && y < bt->buttons->home->y + bt->buttons->home->height) {
            bt->buttons->home->callback(bt);
        }
    }
}

//Menu Link Izq y Der
void handleRightButtonMenu(LinkElementMenu *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->x && x < bt->x + bt->width && y >= bt->y && y < bt->y + bt->height) {
            bt->callback(bt);
        }
    }
}

void handleLeftButtonMenu(LinkElementMenu *bt, GUITouchState *touch) {
    if (touch->touchDetected) {
        // touch detected...
        uint16_t x = touch->touchX[0];
        uint16_t y = touch->touchY[0];

        if (x >= bt->x && x < bt->x + bt->width && y >= bt->y && y < bt->y + bt->height) {
            bt->callback(bt);
        }
    }
}

//Callbacks
void LinkCallback(PedalElement *e) {
    switch (e->link->name) {
        case DELAY:
            DrawScreen(DELAY);
            pedal_individual = 1;
            selection_pedal = DELAY;
            break;

        case OCTAVADOR:
            DrawScreen(OCTAVADOR);
            pedal_individual = 1;
            selection_pedal = OCTAVADOR;
            break;

        case CHORUS:
            DrawScreen(CHORUS);
            pedal_individual = 1;
            selection_pedal = CHORUS;
            break;

        case PHASER:
            DrawScreen(PHASER);
            selection_pedal = PHASER;
            pedal_individual = 1;
            break;

        case WAH:
            DrawScreen(WAH);
            pedal_individual = 1;
            selection_pedal = WAH;
            break;

        case REVERB:
            DrawScreen(REVERB);
            pedal_individual = 1;
            selection_pedal = REVERB;
            break;

        case DISTORTION:
            DrawScreen(DISTORTION);
            pedal_individual = 1;
            selection_pedal = DISTORTION;
            break;

        case FLANGER:
            DrawScreen(FLANGER);
            pedal_individual = 1;
            selection_pedal = FLANGER;
            break;

        case TREMOLO:
            DrawScreen(TREMOLO);
            pedal_individual = 1;
            selection_pedal = TREMOLO;
            break;

        case VIBRATO:
            DrawScreen(VIBRATO);
            selection_pedal = VIBRATO;
            pedal_individual = 1;
            break;

        case EQ:
            DrawScreen(EQ);
            pedal_individual = 1;
            selection_pedal = EQ;
            break;

        case RINGMOD:
            DrawScreen(RINGMOD);
            pedal_individual = 1;
            selection_pedal = RINGMOD;
            break;
    }
    PedalForceRedraw(e);
}

void PushCallback(PedalElement *e) {
    if (e->push->push_state & GUI_DIRTY) {
        uint8_t states = e->push->push_state & GUI_ONOFF_MASK;

        if (pedal_individual == 0) {
            if (states == GUI_ON) {
                switch (e->push->push_menu->name) {
                    case DELAY:
                        memcpy(buffer_images, (uint8_t *)ledprendidodelaymenu, 100);
                        drawBitmapRaw(85, 96, 5, 5, buffer_images, CM_ARGB8888, 1);
                        break;

                    case OCTAVADOR:
                        memcpy(buffer_images, (uint8_t *)ledprendidooctavadormenu, 144);
                        drawBitmapRaw(237, 78, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case CHORUS:
                        memcpy(buffer_images, (uint8_t *)ledprendidochorusmenu, 36);
                        drawBitmapRaw(375, 18, 3, 3, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case PHASER:
                        memcpy(buffer_images, (uint8_t *)ledprendidophasermenu, 120);
                        drawBitmapRaw(103, 210, 5, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case WAH:
                        memcpy(buffer_images, (uint8_t *)ledprendidowahmenu, 224);
                        drawBitmapRaw(236, 209, 8, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case REVERB:
                        memcpy(buffer_images, (uint8_t *)ledprendidoreverbmenu, 224);
                        drawBitmapRaw(375, 211, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case DISTORTION:
                        memcpy(buffer_images, (uint8_t *)ledprendidoDISTORTIONmenu, 100);
                        drawBitmapRaw(80, 95, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case FLANGER:
                        memcpy(buffer_images, (uint8_t *)ledprendidoflangermenu, 144);
                        drawBitmapRaw(237, 65, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case TREMOLO:
                        memcpy(buffer_images, (uint8_t *)ledprendidotremolomenu, 256);
                        drawBitmapRaw(390, 22, 8, 8, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case VIBRATO:
                        memcpy(buffer_images, (uint8_t *)ledprendidovibratomenu, 100);
                        drawBitmapRaw(80, 232, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case EQ:
                        memcpy(buffer_images, (uint8_t *)ledprendidoeqmenu, 84);
                        drawBitmapRaw(190, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(209, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(228, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(247, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(267, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(286, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case RINGMOD:
                        memcpy(buffer_images, (uint8_t *)ledprendidoringmodmenu, 144);
                        drawBitmapRaw(373, 241, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case GRABAR:
                        memcpy(buffer_images, (uint8_t *)cartel, 196000);
                        drawBitmapRaw(65, 66, 350, 140, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;
                }
            } else {
                switch (e->push->push_menu->name) {
                    case DELAY:
                        memcpy(buffer_images, (uint8_t *)ledapagadodelaymenu, 100);
                        drawBitmapRaw(85, 96, 5, 5, buffer_images, CM_ARGB8888, 1);
                        break;

                    case OCTAVADOR:
                        memcpy(buffer_images, (uint8_t *)ledapagadooctavadormenu, 144);
                        drawBitmapRaw(237, 78, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case CHORUS:
                        memcpy(buffer_images, (uint8_t *)ledapagadochorusmenu, 36);
                        drawBitmapRaw(375, 18, 3, 3, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case PHASER:
                        memcpy(buffer_images, (uint8_t *)ledapagadophasermenu, 120);
                        drawBitmapRaw(103, 210, 5, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case WAH:
                        memcpy(buffer_images, (uint8_t *)ledapagadowahmenu, 624);
                        drawBitmapRaw(233, 205, 13, 12, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case REVERB:
                        memcpy(buffer_images, (uint8_t *)ledapagadoreverbmenu, 100);
                        drawBitmapRaw(375, 211, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case DISTORTION:
                        memcpy(buffer_images, (uint8_t *)ledapagadoDISTORTIONmenu, 100);
                        drawBitmapRaw(80, 95, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case FLANGER:
                        memcpy(buffer_images, (uint8_t *)ledapagadoflangermenu, 144);
                        drawBitmapRaw(237, 65, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case TREMOLO:
                        memcpy(buffer_images, (uint8_t *)ledapagadotremolomenu, 256);
                        drawBitmapRaw(390, 22, 8, 8, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case VIBRATO:
                        memcpy(buffer_images, (uint8_t *)ledapagadovibratomenu, 100);
                        drawBitmapRaw(80, 232, 5, 5, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case EQ:
                        memcpy(buffer_images, (uint8_t *)ledapagadoeqmenu, 84);
                        drawBitmapRaw(190, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(209, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(228, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(247, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(267, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        drawBitmapRaw(286, 190, 3, 7, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case RINGMOD:
                        memcpy(buffer_images, (uint8_t *)ledapagadoringmodmenu, 144);
                        drawBitmapRaw(373, 241, 6, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case GRABAR:
//									  DrawScreen(GRABAR);
                        memset(buffer_images, 0, 196000);
                        memset(buffer_images, 0, 196000);
                        drawBitmapRaw(65, 66, 350, 140, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;
                }
            }
        } else {
            if (states == GUI_ON) {
                switch (e->push->push_indiv->name) {
                    case DELAY:
                        memcpy(buffer_images, (uint8_t *)ledprendidodelaypedal, 360);
                        drawBitmapRaw(196, 200, 10, 9, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case OCTAVADOR:
                        memcpy(buffer_images, (uint8_t *)ledprendidooctavadorpedal, 960);
                        drawBitmapRaw(233, 159, 15, 16, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case CHORUS:
                        memcpy(buffer_images, (uint8_t *)ledprendidochoruspedal, 168);
                        drawBitmapRaw(236, 23, 7, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case PHASER:
                        memcpy(buffer_images, (uint8_t *)ledprendidophaserpedal, 576);
                        drawBitmapRaw(235, 150, 12, 12, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case WAH:
                        memcpy(buffer_images, (uint8_t *)ledprendidowahpedal, 1368);
                        drawBitmapRaw(230, 148, 19, 18, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case REVERB:
                        memcpy(buffer_images, (uint8_t *)ledprendidoreverbpedal, 676);
                        drawBitmapRaw(236, 151, 13, 13, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case DISTORTION:
                        memcpy(buffer_images, (uint8_t *)ledprendidoDISTORTIONpedal, 324);
                        drawBitmapRaw(198, 198, 9, 9, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case FLANGER:
                        memcpy(buffer_images, (uint8_t *)ledprendidoflangerpedal, 676);
                        drawBitmapRaw(234, 130, 13, 13, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case TREMOLO:
                        memcpy(buffer_images, (uint8_t *)ledprendidotremolopedal, 2208);
                        drawBitmapRaw(228, 29, 23, 24, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case VIBRATO:
                        memcpy(buffer_images, (uint8_t *)ledprendidovibratopedal, 440);
                        drawBitmapRaw(196, 200, 11, 10, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case EQ:
                        PedalForceRedraw(e);
                        break;

                    case RINGMOD:
                        memcpy(buffer_images, (uint8_t *)ledprendidoringmodpedal, 400);
                        drawBitmapRaw(195, 222, 10, 10, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;
                }
            } else {
                switch (e->push->push_indiv->name) {
                    case DELAY:
                        memcpy(buffer_images, (uint8_t *)ledapagadodelaypedal, 360);
                        drawBitmapRaw(196, 200, 10, 9, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case OCTAVADOR:
                        memcpy(buffer_images, (uint8_t *)ledapagadooctavadorpedal, 960);
                        drawBitmapRaw(233, 159, 15, 16, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case CHORUS:
                        memcpy(buffer_images, (uint8_t *)ledapagadochoruspedal, 168);
                        drawBitmapRaw(236, 23, 7, 6, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case PHASER:
                        memcpy(buffer_images, (uint8_t *)ledapagadophaserpedal, 576);
                        drawBitmapRaw(235, 150, 12, 12, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case WAH:
                        memcpy(buffer_images, (uint8_t *)ledapagadowahpedal, 1368);
                        drawBitmapRaw(230, 148, 19, 18, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case REVERB:
                        memcpy(buffer_images, (uint8_t *)ledapagadoreverbpedal, 676);
                        drawBitmapRaw(236, 151, 13, 13, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case DISTORTION:
                        memcpy(buffer_images, (uint8_t *)ledapagadoDISTORTIONpedal, 324);
                        drawBitmapRaw(198, 198, 9, 9, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case FLANGER:
                        memcpy(buffer_images, (uint8_t *)ledapagadoflangerpedal, 676);
                        drawBitmapRaw(234, 130, 13, 13, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case TREMOLO:
                        memcpy(buffer_images, (uint8_t *)ledapagadotremolopedal, 2208);
                        drawBitmapRaw(228, 29, 23, 24, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case VIBRATO:
                        memcpy(buffer_images, (uint8_t *)ledapagadovibratopedal, 440);
                        drawBitmapRaw(196, 200, 11, 10, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;

                    case EQ:
                        PedalForceRedraw(e);
                        break;

                    case RINGMOD:
                        memcpy(buffer_images, (uint8_t *)ledapagadoringmodpedal, 400);
                        drawBitmapRaw(195, 222, 10, 10, (uint8_t *)buffer_images, CM_ARGB8888, 1);
                        break;
                }
            }
        }

        e->push->push_state &= ~((uint16_t)GUI_DIRTY);
    }
}

void LinkRightCallbackPedal(PedalElement *e) {
    selection_pedal++;

    if (selection_pedal >= 0 && selection_pedal < 6) {
        seleccion_menu = MENU_1;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    } else if (selection_pedal >= 6 && selection_pedal < 12) {
        seleccion_menu = MENU_2;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    } else if (selection_pedal == 12) {
        seleccion_menu = MENU_1;
        selection_pedal = 0;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    }
}

void LinkLeftCallbackPedal(PedalElement *e) {
    selection_pedal--;

    if (selection_pedal >= 0 && selection_pedal < 6) {
        seleccion_menu = MENU_1;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    } else if (selection_pedal >= 6 && selection_pedal < 12) {
        seleccion_menu = MENU_2;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    } else if (selection_pedal == -1) {
        seleccion_menu = MENU_2;
        selection_pedal = 11;
        DrawScreen(selection_pedal);
        PedalForceRedraw(pedal_effects[selection_pedal]);
    }
}

void LinkHomeCallbackPedal(PedalElement *e) {
    if (seleccion_menu == MENU_1) {
        pedal_individual = 0;
        DrawScreen(MENU_1);
        MenuForceRedraw();
        selection_pedal = 20;
    } else {
        pedal_individual = 0;
        DrawScreen(MENU_2);
        MenuForceRedraw();
        selection_pedal = 20;
    }
}

void LinkDerechaCallbackMenu(LinkElementMenu *e) {
    if (seleccion_menu == MENU_1) {
        seleccion_menu = MENU_2;
        DrawScreen(MENU_2);
    } else if (seleccion_menu == MENU_2) {
        seleccion_menu = MENU_3;
        DrawScreen(MENU_3);
    }

    MenuForceRedraw();
}

void LinkIzquierdaCallbackMenu(LinkElementMenu *e) {
    if (seleccion_menu == MENU_2) {
        seleccion_menu = MENU_1;
        DrawScreen(MENU_1);
    } else if (seleccion_menu == MENU_3) {
        seleccion_menu = MENU_2;
        DrawScreen(MENU_2);
    }

    MenuForceRedraw();
}

GUI * initGUI(uint8_t num, sFONT *font, uint32_t bgCol, uint32_t textCol) {
    GUI *gui = (GUI *)calloc(1, sizeof(GUI));

    gui->items = (GUIElement **)calloc(num, sizeof(GUIElement *));
    gui->numItems = num;
    gui->font = font;
    gui->bgColor = bgCol;
    gui->textColor = textCol;
    return gui;
}

void initPedals() {
    // = { .pixels = Perillas_Buffer_35x35x25,
    //.spriteWidth = 35, .spriteHeight = 35, .numSprites = 25, .format =
    //CM_ARGB8888};//CM_RGB888
    perilla3535.format = CM_ARGB8888;
    perilla3535.numSprites = 25;
    perilla3535.pixels = Perillas_Buffer_35x35x25;
    perilla3535.spriteHeight = 35;
    perilla3535.spriteWidth = 35;

    //= { .pixels = Perillas_Buffer_42x41x25,
    //.spriteWidth = 42, .spriteHeight = 41, .numSprites = 25, .format =
    //CM_ARGB8888};
    perilla4241.format = CM_ARGB8888;
    perilla4241.numSprites = 25;
    perilla4241.pixels = Perillas_Buffer_42x41x25;
    perilla4241.spriteHeight = 41;
    perilla4241.spriteWidth = 42;

    // = { .pixels = perilla52x52x25,
    //.spriteWidth = 52, .spriteHeight = 52, .numSprites = 25, .format =
    //CM_ARGB8888};//CM_RGB888
    perilla5252.format = CM_ARGB8888;
    perilla5252.numSprites = 25;
    perilla5252.pixels = Perillas_Buffer_52x52x25;
    perilla5252.spriteHeight = 52;
    perilla5252.spriteWidth = 52;

    //= { .pixels = eq_slider_30,
    //.spriteWidth = 30, .spriteHeight = 83, .numSprites = 26, .format =
    //CM_ARGB8888};//CM_RGB888
    eq_slider.format = CM_ARGB8888;
    eq_slider.numSprites = 26;
    eq_slider.pixels = Perillas_Buffer_eq_slider;
    eq_slider.spriteHeight = 83;
    eq_slider.spriteWidth = 30;

    //Initialize Screen1 Knobs
    //Delay
    pedal_effects[0] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[0]->dial = initKnob(3);
    pedal_effects[0]->dial->dials[0] = guiDialButton(0, "", 174, 36, 0.0f, 0.045f, KNOB, &knob5252, delayFeedback);
    pedal_effects[0]->dial->dials[1] = guiDialButton(1, "", 253, 36, 0.400f, 0.045f, KNOB, &knob5252, delayTime);
    pedal_effects[0]->dial->dials[2] = guiDialButton(2, "", 220, 82, 0.5f, 0.045f, KNOB, &knob4241, delayLevel);
    //Octavador
    pedal_effects[1] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[1]->dial = initKnob(1);
    pedal_effects[1]->dial->perillas[0] = guiDialButton(8, "", 0, 0, 0.0f, 0.045f, KNOB, &knob5252, NULL);;
    //Chorus
    pedal_effects[2] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[2]->dial = initKnob(2);
    pedal_effects[2]->dial->dials[0] = guiDialButton(0, "", 187, 22, 0.6f, 0.045f, KNOB, &knob4241, chorusRate);
    pedal_effects[2]->dial->dials[1] = guiDialButton(1, "", 250, 22, 0.367f, 0.045f, KNOB, &knob4241, chorusDepth);
    //Phaser
    pedal_effects[3] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[3]->dial = initKnob(1);
    pedal_effects[3]->dial->dials[0] = guiDialButton(0, "", 214, 27, 0.667f, 0.045f, KNOB, &knob5252, phaserRate);
    //Autowah
    pedal_effects[4] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[4]->dial = initKnob(4);
    pedal_effects[4]->dial->dials[0] = guiDialButton(0, "", 119, 41, 0.43f, 0.045f, KNOB, &knob4241, autowahDepth);
    pedal_effects[4]->dial->dials[1] = guiDialButton(1, "", 186, 41, 0.33f, 0.045f, KNOB, &knob4241, autowahRate);
    pedal_effects[4]->dial->dials[2] = guiDialButton(2, "", 251, 41, 0.6f, 0.045f, KNOB, &knob4241, autowahVolume);
    pedal_effects[4]->dial->dials[3] = guiDialButton(3, "", 319, 41, 0.9f, 0.045f, KNOB, &whaonda, autowahMod);
    //Reverb
    pedal_effects[5] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[5]->dial = initKnob(2);
    pedal_effects[5]->dial->dials[0] = guiDialButton(0, "", 191, 47, 0.5f, 0.045f, KNOB, &knob4241, reverbDecay);
    pedal_effects[5]->dial->dials[1] = guiDialButton(1, "", 253, 47, 0.4f, 0.045f, KNOB, &knob4241, reverbMix);

    //Inicializo las perillas Pantalla2
    //DISTORTION
    pedal_effects[6] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[6]->dial = initKnob(2);
    pedal_effects[6]->dial->dials[0] = guiDialButton(0, "", 179, 26, 0.61f, 0.045f, KNOB, &knob5252, DISTORTIONGain);
    pedal_effects[6]->dial->dials[1] = guiDialButton(1, "", 249, 26, 0.5f, 0.045f, KNOB, &knob5252, DISTORTIONBlend);
    //Flanger
    pedal_effects[7] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[7]->dial = initKnob(4);
    pedal_effects[7]->dial->dials[0] = guiDialButton(0, "", 116, 55, 0.0f, 0.045f, KNOB, &knob5252, flangerManual);
    pedal_effects[7]->dial->dials[1] = guiDialButton(1, "", 182, 55, 0.57f, 0.045f, KNOB, &knob5252, flangerDepth);
    pedal_effects[7]->dial->dials[2] = guiDialButton(2, "", 246, 55, 0.2f, 0.045f, KNOB, &knob5252, flangerRate);
    pedal_effects[7]->dial->dials[3] = guiDialButton(3, "", 311, 55, 0.81f, 0.045f, KNOB, &knob5252, flangerFeedback);
    //Tremolo
    pedal_effects[8] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[8]->dial = initKnob(3);
    pedal_effects[8]->dial->dials[0] = guiDialButton(0, "", 167, 40, 0.67f, 0.045f, KNOB, &knob4241, tremoloDepth);
    pedal_effects[8]->dial->dials[1] = guiDialButton(1, "", 220, 66, 0.2f, 0.045f, KNOB, &tremoloonda, tremoloMod);
    pedal_effects[8]->dial->dials[2] = guiDialButton(2, "", 269, 40, 0.5f, 0.045f, KNOB, &knob4241, tremoloRate);
    //Vibrato
    pedal_effects[9] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[9]->dial = initKnob(3);
    pedal_effects[9]->dial->dials[0] = guiDialButton(0, "", 180, 43, 0.6f, 0.045f, KNOB, &knob4241, vibratoRate);
    pedal_effects[9]->dial->dials[1] = guiDialButton(1, "", 257, 43, 0.41f, 0.045f, KNOB, &knob4241, vibratoDepth);
    pedal_effects[9]->dial->dials[2] = guiDialButton(2, "", 223, 86, 1.0f, 0.045f, KNOB, &vibratoonda, vibratoMod);
    //EQ
    pedal_effects[10] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[10]->dial = initKnob(6);
    pedal_effects[10]->dial->dials[0] = guiDialButton(0, "", 111, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain0);
    pedal_effects[10]->dial->dials[1] = guiDialButton(1, "", 156, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain1);
    pedal_effects[10]->dial->dials[2] = guiDialButton(2, "", 201, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain2);
    pedal_effects[10]->dial->dials[3] = guiDialButton(3, "", 246, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain3);
    pedal_effects[10]->dial->dials[4] = guiDialButton(4, "", 291, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain4);
    pedal_effects[10]->dial->dials[5] = guiDialButton(5, "", 336, 97, 0.5f, 0.045f, SLIDER, &eq_slider, eqGain5);
    //Ringmod
    pedal_effects[11] = (PedalElement *)calloc(1, sizeof(PedalElement));
    pedal_effects[11]->dial = initKnob(1);
    pedal_effects[11]->dial->dials[0] = guiDialButton(0, "", 223, 73, 0.179f, 0.045f, KNOB, &knob3535, ringmodRate);

    //Inicializo los Push (3pdt) Pantalla 1
    //pedal_effects Menu
    pedal_effects[0]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[0]->push->push_menu = initPushLink(DELAY, 76, 13 + 110 / 2, 58, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[1]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[1]->push->push_menu = initPushLink(OCTAVADOR, 198, 13 + 110 / 2, 84, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[2]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[2]->push->push_menu = initPushLink(CHORUS, 346, 13 + 110 / 2, 61, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[3]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[3]->push->push_menu = initPushLink(PHASER, 73, 149 + 110 / 2, 64, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[4]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[4]->push->push_menu = initPushLink(WAH, 190, 160 + 88 / 2, 100, 88 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[5]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[5]->push->push_menu = initPushLink(REVERB, 343, 149 + 110 / 2, 67, 110 / 2, PushCallback, handlePushMenuButton);

    //Inicializo los Push (3pdt) Pantalla2
    //pedal_effects Menu
    pedal_effects[6]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[6]->push->push_menu = initPushLink(DISTORTION, 70, 13 + 110 / 2, 59, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[7]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[7]->push->push_menu = initPushLink(FLANGER, 179, 23 + 90 / 2, 121, 90 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[8]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[8]->push->push_menu = initPushLink(TREMOLO, 354, 13 + 110 / 2, 80, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[9]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[9]->push->push_menu = initPushLink(VIBRATO, 71, 149 + 110 / 2, 57, 110 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[10]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[10]->push->push_menu = initPushLink(EQ, 181, 159 + 90 / 2, 118, 90 / 2, PushCallback, handlePushMenuButton);

    pedal_effects[11]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[11]->push->push_menu = initPushLink(RINGMOD, 360, 149 + 110 / 2, 68, 110 / 2, PushCallback, handlePushMenuButton);

    //Inicializo los Push (3pdt) Pantalla2
    //pedal_effects Menu
    pedal_effects[12]->push = (PushElement *)calloc(1, sizeof(PushElement));
    pedal_effects[12]->push->push_menu = initPushLink(RECORD, 100, 91, 280, 90, PushCallback, handlePushMenuButton);

    //pedal_effects Individuales Pantalla1
    pedal_effects[0]->push->push_indiv = initPushLink(DELAY, 225, 189, 31, 33, PushCallback, handlePushIndividualButton);
    pedal_effects[1]->push->push_indiv = initPushLink(OCTAVADOR, 219, 180, 42, 42, PushCallback, handlePushIndividualButton);
    pedal_effects[2]->push->push_indiv = initPushLink(CHORUS, 180, 163, 117, 77, PushCallback, handlePushIndividualButton);
    pedal_effects[3]->push->push_indiv = initPushLink(PHASER, 222, 171, 37, 37, PushCallback, handlePushIndividualButton);
    pedal_effects[4]->push->push_indiv = initPushLink(WAH, 218, 213, 41, 41, PushCallback, handlePushIndividualButton);
    pedal_effects[5]->push->push_indiv = initPushLink(REVERB, 180, 163, 117, 77, PushCallback, handlePushIndividualButton);

    //pedal_effects Individuales Pantalla2
    pedal_effects[6]->push->push_indiv = initPushLink(DISTORTION, 223, 184, 35, 35, PushCallback, handlePushIndividualButton);
    pedal_effects[7]->push->push_indiv = initPushLink(FLANGER, 223, 168, 37, 37, PushCallback, handlePushIndividualButton);
    pedal_effects[8]->push->push_indiv = initPushLink(TREMOLO, 222, 203, 36, 36, PushCallback, handlePushIndividualButton);
    pedal_effects[9]->push->push_indiv = initPushLink(VIBRATO, 221, 188, 37, 37, PushCallback, handlePushIndividualButton);
    pedal_effects[10]->push->push_indiv = initPushLink(EQ, 218, 186, 37, 41, PushCallback, handlePushIndividualButton);
    pedal_effects[11]->push->push_indiv = initPushLink(RINGMOD, 223, 210, 35, 34, PushCallback, handlePushIndividualButton);
    //pedal_effects estados
    pedal_effects[0]->push->push_state = GUI_OFF;
    pedal_effects[1]->push->push_state = GUI_OFF;
    pedal_effects[2]->push->push_state = GUI_OFF;
    pedal_effects[3]->push->push_state = GUI_OFF;
    pedal_effects[4]->push->push_state = GUI_OFF;
    pedal_effects[5]->push->push_state = GUI_OFF;
    pedal_effects[6]->push->push_state = GUI_OFF;
    pedal_effects[7]->push->push_state = GUI_OFF;
    pedal_effects[8]->push->push_state = GUI_OFF;
    pedal_effects[9]->push->push_state = GUI_OFF;
    pedal_effects[10]->push->push_state = GUI_OFF;
    pedal_effects[11]->push->push_state = GUI_OFF;

    pedal_effects[12]->push->push_state = GUI_OFF;

    //Initialize the pedal_effects links Screen1
    pedal_effects[0]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[0]->link = initPushLink(DELAY, 76, 13, 58, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[1]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[1]->link = initPushLink(OCTAVADOR, 198, 13, 84, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[2]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[2]->link = initPushLink(CHORUS, 346, 13, 61, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[3]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[3]->link = initPushLink(PHASER, 73, 149, 64, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[4]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[4]->link = initPushLink(WAH, 190, 160, 100, 88 / 2, LinkCallback, handleLinkButton);

    pedal_effects[5]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[5]->link = initPushLink(REVERB, 343, 149, 67, 110 / 2, LinkCallback, handleLinkButton);

    //Initialize the pedal_effects links Screen2
    pedal_effects[6]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[6]->link = initPushLink(DISTORTION, 70, 13, 59, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[7]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[7]->link = initPushLink(FLANGER, 179, 23, 121, 90 / 2, LinkCallback, handleLinkButton);

    pedal_effects[8]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[8]->link = initPushLink(TREMOLO, 354, 13, 80, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[9]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[9]->link = initPushLink(VIBRATO, 71, 149, 57, 110 / 2, LinkCallback, handleLinkButton);

    pedal_effects[10]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[10]->link = initPushLink(EQ, 181, 159, 118, 90 / 2, LinkCallback, handleLinkButton);

    pedal_effects[11]->link = (LinkElement *)calloc(1, sizeof(LinkElement));
    pedal_effects[11]->link = initPushLink(RINGMOD, 360, 149, 68, 110 / 2, LinkCallback, handleLinkButton);

    //Initialize the buttons of the individual screens of the pedal_effects SCREEN1
    pedal_effects[0]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[0]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[0]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[0]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[1]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[1]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[1]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[1]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[2]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[2]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[2]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[2]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[3]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[3]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[3]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[3]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[4]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[4]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[4]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[4]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[5]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[5]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[5]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[5]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    //Initialize the buttons of the individual screens of the pedal_effects SCREEN2
    pedal_effects[6]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[6]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[6]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[6]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[7]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[7]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[7]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[7]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[8]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[8]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[8]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[8]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[9]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[9]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[9]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[9]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[10]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[10]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[10]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[10]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    pedal_effects[11]->buttons = (MenuElement *)calloc(1, sizeof(MenuElement));
    pedal_effects[11]->buttons->rightArrow = initPushLink(RIGHT, 417, 84, 61, 107, LinkRightCallbackPedal, handleRightButtonPedal);
    pedal_effects[11]->buttons->leftArrow = initPushLink(LEFT, 0, 84, 61, 107, LinkLeftCallbackPedal, handleLeftButtonPedal);
    pedal_effects[11]->buttons->home = initPushLink(HOME, 0, 0, 70, 70, LinkHomeCallbackPedal, handleHomeButtonPedal);

    //Link Menu Left and Right
    Left_Menu_Arrow = (LinkElementMenu *)calloc(1, sizeof(LinkElementMenu));
    Left_Menu_Arrow = initPushLinkMenu(LEFT_MENU, 5, 101, 39, 70, leftMenuCallback, handleLeftMenuButton); 
    Right_Menu_Arrow = (LinkElementMenu *)calloc(1, sizeof(LinkElementMenu));
    Right_Menu_Arrow = initPushLinkMenu(RIGHT_MENU, 435, 101, 39, 70, rightMenuCallback, handleRightMenuButton);

    //initialize the pedal_effects with their SCREEN1 functions
    pedal_effects[0]->effect = delayEffect;
    pedal_effects[1]->effect = octaverEffect;
    pedal_effects[2]->effect = chorusEffect;
    pedal_effects[3]->effect = phaserEffect;
    pedal_effects[4]->effect = autowahEffect;
    pedal_effects[5]->effect = reverbEffect;

    //Initialize the pedal_effects with their DISPLAY2 functions
    pedal_effects[6]->effect = distortionEffect;
    pedal_effects[7]->effect = flangerEfecto;
    pedal_effects[8]->effect = tremoloEffect;
    pedal_effects[9]->effect = vibratoEffect;
    pedal_effects[10]->effect = eqEffect;
    pedal_effects[11]->effect = ringmodEffect;

    //Initialize the pedal_effects with their DISPLAY3 functions

    pedal_effects[12]->effect = grabar_wav;
}

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

int grabar_wav(int enetrada) {
    return 0;
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