#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "tonc.h"

#define KEYBOARD_X 2
#define KEYBOARD_WIDTH 26
#define KEYBOARD_HEIGHT 8

#define BACKSPACE_TILE 40
#define BACKSPACE_CHAR 126 //ascii for ~, which is being mapped the backspace
#define SPACE 0x20         //ascii space...

#define TILE_SIZE 8
#define CUR_OFFS 16
#define KEYBOARD_INPUT_STATE 1

typedef struct Keyboard
{
    bool visible;
    int cursorX;
    int cursorY;
    int anchorX;
    int anchorY;
    int textX;
    int x;
    int y;
    int height;
    int width;
} Keyboard;

#define KUP 0
#define KDOWN 1
#define KLEFT 2
#define KRIGHT 3

extern Keyboard *keyboard;

void initKeyboard(int x, int y);
int inputKeyboard();
void alterString(char **str, char a);
void drawKeyboard();
void drawCursor();
void moveCursor();
void enableKeyboard();
void setCoordinates(int x, int y);
void hideKeyboard();

#endif