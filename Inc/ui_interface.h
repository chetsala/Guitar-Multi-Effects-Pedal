#pragma once

#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
//#include "gui/link.h"

#define UI_BG_COLOR    ((uint32_t)0xff59626c)  // Background color for the UI
#define UI_TEXT_COLOR  ((uint32_t)0xffcccccc)  // Text color for the UI
#define UI_FONT        Font12                  // Default font for the UI
#define UI_SENSITIVITY 0.02f                   // Sensitivity for touch interactions

typedef struct {
    uint16_t touchX[1];        // X-coordinate of the touch
    uint16_t touchY[1];        // Y-coordinate of the touch
    uint32_t lastTouch;        // Timestamp of the last touch
    uint8_t touchDetected;     // Whether a touch was detected
    uint8_t touchUpdate;       // Whether the touch state was updated
} GUITouchState;

// Forward declaration for function pointers below
typedef struct GUIElement GUIElement;

typedef void (*GUIElementHandler)(GUIElement *button, GUITouchState *touchState);
typedef void (*GUIElementRenderer)(GUIElement *button);
typedef void (*GUICallback)(GUIElement *button);

// Element state flags
enum {
    GUI_OFF        = 1,  // Element is off
    GUI_ON         = 2,  // Element is on
    GUI_HOVER      = 4,  // Element is being hovered over
    GUI_DIRTY      = 8,  // Element needs re-rendering
    GUI_ONOFF_MASK = GUI_ON | GUI_OFF  // Mask for on/off states
};

typedef struct {
    const uint8_t *pixels;   // Pointer to pixel data
    uint16_t spriteWidth;    // Width of each sprite
    uint16_t spriteHeight;   // Height of each sprite
    uint8_t numSprites;      // Number of sprites in the sheet
    uint32_t format;         // Format of the sprite data
} SpriteSheet;

struct GUIElement {
    uint16_t x;                 // X-coordinate of the element
    uint16_t y;                 // Y-coordinate of the element
    uint16_t width;             // Width of the element
    uint16_t height;            // Height of the element
    uint32_t state;             // Current state of the element
    char *label;                // Label text for the element
    void *userData;             // User data associated with the element
    SpriteSheet *sprite;        // Pointer to sprite sheet for the element
    GUIElementHandler handler;  // Handler function for interactions
    GUIElementRenderer render;  // Render function for the element
    GUICallback callback;       // Callback function for the element
    uint8_t id;                 // Identifier for the element
};

typedef struct {
    float value;            // Value of the dial
    float startValue;       // Starting value when touched
    float sensitivity;      // Sensitivity to touch movements
    uint16_t startX;        // X-coordinate when touch started
    uint16_t startY;        // Y-coordinate when touch started
    uint8_t orientation;    // Orientation of the dial
} DialButtonState;

typedef struct {
    float value;            // Value of the push button
} PushButtonState;

typedef struct {
    GUIElement **items;     // List of items in the GUI
    uint32_t bgColor;       // Background color of the GUI
    uint32_t textColor;     // Text color of the GUI
    sFONT *font;            // Font used in the GUI
    uint8_t numItems;       // Number of items in the GUI
    int8_t selected;        // Index of the selected item
} GUI;

void handlePushButton(GUIElement *bt, GUITouchState *touch);
void renderPushButton(GUIElement *bt);

void handleDialButton(GUIElement *bt, GUITouchState *touch);
void renderDialButton(GUIElement *bt);

void init_LL_ConvertLine_DMA2D(uint32_t color_mode);
void LL_ConvertLineToARGB8888(void *src, void *dst, uint32_t xstride, uint32_t color_mode);
void drawSprite(uint16_t x, uint16_t y, uint8_t id, SpriteSheet *sprite);
void drawBitmapRaw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pixels, uint32_t colorMode, uint8_t active_label);

void LCD_Config(void);
void InitializeDemoScreen(void);
void DrawScreen(int num);

GUI * initGUI(uint8_t num, sFONT *font, uint32_t bgCol, uint32_t textCol);
void guiForceRedraw(GUI *gui);

void guiUpdateTouch(TS_StateTypeDef *raw, GUITouchState *touch);

GUIElement * guiElementCommon(uint8_t id, char *label, uint16_t x, uint16_t y, SpriteSheet *sprite, GUICallback cb);
GUIElement * guiPushButton(uint8_t id, char *label, uint16_t x, uint16_t y, float val, SpriteSheet *sprite, GUICallback cb);
GUIElement * guiDialButton(uint8_t id, char *label, uint16_t x, uint16_t y, float val, float sens, uint8_t orientation, SpriteSheet *sprite, GUICallback cb);

// LINK Elements for additional functionality
typedef struct LinkElement LinkElement;
typedef struct LinkElementMenu LinkElementMenu;
typedef struct PedalElement PedalElement;
typedef void (*LinkHandler)(PedalElement *button, GUITouchState *touchState);
typedef void (*LinkHandlerMenu)(LinkElementMenu *button, GUITouchState *touchState);
typedef void (*GUICallbackLink)(PedalElement *button);
typedef void (*GUICallbackLinkMenu)(LinkElementMenu *button);
typedef int (*EffectFunction)(int input);

struct LinkElement {
    uint8_t name;          // Name or ID of the link element
    uint16_t x;            // X-coordinate of the element
    uint16_t y;            // Y-coordinate of the element
    uint16_t width;        // Width of the element
    uint16_t height;       // Height of the element
    GUICallbackLink callback;  // Callback function for interactions
    LinkHandler handler;       // Handler for interactions
};

struct LinkElementMenu {
    uint8_t name;          // Name or ID of the menu link element
    uint16_t x;            // X-coordinate of the element
    uint16_t y;            // Y-coordinate of the element
    uint16_t width;        // Width of the element
    uint16_t height;       // Height of the element
    GUICallbackLinkMenu callback;  // Callback function for menu interactions
    LinkHandlerMenu handler;       // Handler for menu interactions
};

typedef struct {
    GUIElement **dials;    // Array of dial elements
    uint8_t numItems;      // Number of dial items
} DialElement;

typedef struct {
    LinkElement *menuButton;
    LinkElement *individualButton;
    int buttonState;
} PushElement;

typedef struct {
    LinkElement *leftArrow;
    LinkElement *rightArrow;
    LinkElement *homeButton;
} MenuElement;

struct PedalElement {
    DialElement *dial;         // Dial element in the pedal
    PushElement *push;         // Push button element in the pedal
    LinkElement *link;         // Link element in the pedal
    MenuElement *buttons;      // Menu button elements in the pedal
    EffectFunction effect;     // Effect function applied by the pedal
};

// Custom functions
void readDialFiles(void);
void showBackgroundDemo(void);
void drawScreen(int num);
void menuRequestHandler(PedalElement **buttons, GUITouchState *touch);
void leftArrowMenuRequestHandler(LinkElementMenu *button, GUITouchState *touch);
void individualPedalRequestHandler(PedalElement *button, GUITouchState *touch);
LinkElement * initPushLink(uint8_t name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, GUICallbackLink cb, LinkHandler handler);
LinkElementMenu * initPushLinkMenu(uint8_t name, uint16_t x, uint16_t y, uint16_t width, uint16_t height, GUICallbackLinkMenu cb, LinkHandlerMenu handler);
DialElement * initDial(uint8_t num);
void initPedals(void);

// Handlers
void handleMenuPushButton(PedalElement *button, GUITouchState *touch);
void handleIndividualPushButton(PedalElement *button, GUITouchState *touch);
void handleLinkButton(PedalElement *button, GUITouchState *touch);
void handleRightPedalButton(PedalElement *button, GUITouchState *touch);
void handleLeftPedalButton(PedalElement *button, GUITouchState *touch);
void handleHomePedalButton(PedalElement *button, GUITouchState *touch);
void handleRightMenuButton(LinkElementMenu *button, GUITouchState *touch);
void handleLeftMenuButton(LinkElementMenu *button, GUITouchState *touch);

// Callbacks
void linkCallback(PedalElement *element);
void pushCallback(PedalElement *element);
void rightPedalCallback(PedalElement *element);
void leftPedalCallback(PedalElement *element);
void homePedalCallback(PedalElement *element);
void rightMenuCallback(LinkElementMenu *element);
void leftMenuCallback(LinkElementMenu *element);
void menuPushRequestHandler(PedalElement **buttons, GUITouchState *touch);
void forceMenuRedraw(void);
void forcePedalRedraw(PedalElement *gui);

/* Custom written functions */

void guiUpdate(DialElement *gui, GUITouchState *touch);
