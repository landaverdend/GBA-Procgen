#include "titleScreen.h"
#include "tsSheet.h"
#include "keyboard.h"
//#include "graphics.h"
#include <string.h>
#include <stdlib.h>

Keyboard *keyboard;

#define KBT_LEN 4
const char *kbText[KBT_LEN] = 
{
    "A B C   D E F           ~",
    "G H I   J K L     0 1 2  ",
    "M N O   P Q R S   3 4 5  ",
    "T U V   W X Y Z   6 7 8 9"
};

int inputKeyboard()
{
    bool changed = false;
    
    if (key_hit(KEY_A))
    {
        alterString(&menu->nodes[menu->cursor].data, kbText[keyboard->cursorY][keyboard->cursorX]);

        changed = true;
    }
    if (key_hit(KEY_B))
    {
        hideKeyboard();
        return MENU_INPUT_STATE;
    }

    //using tri_bool here was throwing me a compiler error. come back and investigate..
    if (key_hit(KEY_LEFT))
    {
        moveCursor(KLEFT);
        changed = true;
    }
    if (key_hit(KEY_RIGHT))
    {
        moveCursor(KRIGHT);
        changed = true;
    }
    if (key_hit(KEY_DOWN))
    {
        moveCursor(KDOWN);
        changed = true;
    }
    if (key_hit(KEY_UP))
    {
        moveCursor(KUP);
        changed = true;
    }

    if (changed)
    {
        drawCursor();
        drawKeyboard();
        drawMenu();
    }
    return KEYBOARD_INPUT_STATE;
}

void initKeyboard(int x, int y)
{
    keyboard = malloc(sizeof(Keyboard));
    keyboard->visible = false;
    keyboard->width = KEYBOARD_WIDTH;
    keyboard->height = KEYBOARD_HEIGHT;
    keyboard->x = x;
    keyboard->y = y;
}

void moveCursor(int dir)
{
    int cx = keyboard->cursorX;
    int cy = keyboard->cursorY;
    int rowLength = strlen(kbText[cy]);
    int maxh = KBT_LEN - 1;
    switch (dir)
    {
    case KLEFT:
        cx = ((cx - 2) < 0) ? rowLength - 1 : cx - 2;
        keyboard->cursorX = cx;
        break;
    case KRIGHT:
        cx = ((cx + 2) >= rowLength) ? 0 : cx + 2;
        keyboard->cursorX = cx;
        break;
    case KUP:;
        keyboard->cursorY = (cy - 1) < 0 ? maxh : cy - 1;
        break;
    case KDOWN:
        keyboard->cursorY = (cy + 1) > maxh ? 0 : cy + 1;
        break;
    }
}

void drawCursor()
{
    obj_set_pos(titleCursor_OBJ, keyboard->anchorX + 8 * keyboard->cursorX, keyboard->anchorY + 16 * keyboard->cursorY);
}

void drawKeyboard()
{
    drawMenuBox(keyboard->x, keyboard->y, KEYBOARD_WIDTH, KEYBOARD_HEIGHT);
    drawCursor();
    int yoff = 1;
    for (int i = 0; i < KBT_LEN; i++)
    {
        drawMenuString(keyboard->textX, keyboard->y + yoff, kbText[i]);
        yoff += 2;
    }
}

//pass in the dst to alter, then append character in to the string. cpy to dst
void alterString(char **dst, char in)
{
    int len = strlen(*dst);
    char new[len];
    strcpy(new, *dst);

    if (in == BACKSPACE_CHAR)
    {
        for (int i = len - 1; i > 0; i--)
        {
            new[i] = new[i - 1];
        }
        new[0] = ' ';
    }
    else if (*dst[0] == ' ') //string is not at capacity.
    {
        for (int i = 1; i < len; i++)
        {
            new[i - 1] = new[i];
        }
        new[len - 1] = in;
    }
    *dst = strdup(new);
}

void hideKeyboard()
{
    obj_hide(titleCursor_OBJ);
    keyboard->visible = false;
    eraseBox(keyboard->x, keyboard->y, keyboard->width, keyboard->height);

    drawMenu();
}

//just a helper that resets all coordinates based on coords passed
void setCoordinates(int x, int y)
{
    keyboard->x = x;
    keyboard->y = y;
    keyboard->cursorX = 0; //grid location of cursor
    keyboard->cursorY = 0;
    keyboard->anchorX = keyboard->x * TILE_SIZE + 7; //coords for cursor OAM object. These coordinates map to [A] on keypad
    keyboard->anchorY = keyboard->y * TILE_SIZE + 6;
    keyboard->textX = keyboard->x + 1;
}

void enableKeyboard()
{
    obj_unhide(titleCursor_OBJ, ATTR0_REG);
    keyboard->visible = true;
    setCoordinates(KEYBOARD_X, menu->y + menu->cursor * 2 + 3);
}