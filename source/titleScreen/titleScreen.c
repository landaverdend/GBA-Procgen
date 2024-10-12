#include "titleScreen.h"
#include "tonc.h"
#include "titleSheet.h"
#include "tsSheet.h"
#include "titleSprites.h"
#include "titleCtx.h"
#include "miscHelp.h"
#include "keyboard.h"
#include <stdlib.h>
#include <string.h>

#define BG_1SB 24
#define BG_2SB 28

#define BG_1SIZE 32 * 32 //64 * 64

OBJ_ATTR title_obj_buffer[128];
OBJ_ATTR *titleCursor_OBJ;

Menu *menu;
bool malloced = false;

const iFP TITLE_INPUTS[2] = {inputTitleScreen, inputKeyboard};

int titleInputState = MENU_INPUT_STATE;

bool launch = false;

void initTitleDisplay()
{
    REG_BG1CNT = BG_PRIO(0) | BG_CBB(0) | BG_SBB(BG_1SB) | BG_8BPP | BG_REG_32x32; // Text layer
    REG_BG2CNT = BG_PRIO(2) | BG_CBB(0) | BG_SBB(BG_2SB) | BG_8BPP | BG_AFF_32x32; // affine map
    REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;
}

void initTitleScreenGFX()
{
    memcpy(&tile_mem[0][0], titleSheetTiles, titleSheetTilesLen);
    memcpy(pal_bg_mem, titleSheetPal, titleSheetPalLen);

    memcpy(pal_obj_mem, titleSpritesPal, titleSpritesPalLen);
    memcpy(&tile_mem[4][0], titleSpritesTiles, titleSpritesTilesLen);
    oam_init(title_obj_buffer, 128);
    initTitleObjects();

    for (int i = 0; i < BG_1SIZE; i++)
    {
        se_mem[BG_1SB][i] = BLANK_TILE;
    }
}

void initTitleObjects()
{
    titleCursor_OBJ = obj_set_attr(&title_obj_buffer[0], ATTR0_SQUARE | ATTR0_Y(62), ATTR1_SIZE_16 | ATTR1_X(71), CURSOR_TILE_INDEX | ATTR2_PALBANK(0));
    obj_hide(titleCursor_OBJ);
}

void initTitleScreen(int initialState)
{
    if (!malloced) 
    {
        menu = malloc(sizeof(Menu));
        malloced = true;
        initKeyboard(KEYBOARD_X, menu->y + menu->cursor * 2 + 4);

    }
    setMenuContext(initialState);
}

void updateTitleScreenGFX()
{
    oam_copy(oam_mem, title_obj_buffer, 128);
}

//wrapper for generic title input.
bool inputTitle()
{
    key_poll();

    titleInputState = TITLE_INPUTS[titleInputState]();

    return launch;
}

int inputTitleScreen()
{
    int changed = HIT_VERT_TRI();
    int newState = MENU_INPUT_STATE;
    menu->cursor += changed;

    menu->cursor = (menu->cursor < 0) ? menu->nodesLength - 1 : menu->cursor;
    menu->cursor = (menu->cursor >= menu->nodesLength) ? 0 : menu->cursor;

    if (key_hit(KEY_A))
    {
        changed = 1;
        newState = menu->nodes[menu->cursor].action();
    }

    if (key_hit(KEY_B) && menu->ctx != PAUSE_CTX)
    {
        changed = 1;
        eraseBox(menu->x, menu->y, menu->width, menu->height);
        setMenuContext(ROOT_CTX);
    }

    if (changed)
        drawMenu();

    return newState;
}

void drawMenu()
{
    drawMenuBox(menu->x, menu->y, menu->width, menu->height);
    int xbase = menu->x + 3;
    int ybase = menu->y + 2;
    for (int i = 0; i < menu->nodesLength; i++)
    {
        if (i == menu->cursor)
            drawMenuCursor(xbase - 2, ybase + i * 2);
        
        char *newstr = malloc(strlen(menu->nodes[i].label) + strlen(menu->nodes[i].data) + 1);
        strcpy(newstr, menu->nodes[i].label);
        strcat(newstr, menu->nodes[i].data);
        drawMenuString(xbase, ybase + i * 2, newstr);
        free(newstr);
    }
    if (keyboard->visible)
        drawKeyboard();
}

void eraseBox(int x, int y, int w, int h)
{
    for (int i = 0; i <= w; i++)
    {
        for (int k = 0; k <= h; k++)
        {
            se_mem[BG_1SB][x + i + (k + y) * 32] = BLANK_TILE;
        }
    }
}

void drawMenuCursor(int x, int y)
{
    int base = x + (y * 32);
    se_mem[BG_1SB][base] = MENU_CURSOR;
    se_mem[BG_1SB][base + 1] = BLANK_TILE;
}

void drawMenuString(int x, int y, const char *string)
{
    int length = strlen(string);
    int base = x + (y * 32);
    for (int i = 0; i < length; i++)
    {
        if (string[i] == BACKSPACE_CHAR)
            se_mem[BG_1SB][base + i] = BACKSPACE_TILE;   //backspace on kb
        else if (string[i] >= 0x30 && string[i] <= 0x39) //numeric char
        {
            se_mem[BG_1SB][base + i] = MENU_NUM_BASE + (string[i] - 0x30);
        }
        else
        {
            se_mem[BG_1SB][base + i] = (string[i] == 0x20) ? BLANK_TILE : (MENU_ASCII_BASE + string[i] - 65);
        }
    }
}

void drawMenuBox(int x, int y, int w, int h)
{
    int corner;
    int horz;
    int vert;
    for (int i = 0; i <= w; i++)
    {
        for (int k = 0; k <= h; k++)
        {
            corner = ((i == 0 || i == w) && (k == 0 || k == h));
            vert = ((k == 0 || k == h) && !corner);
            horz = ((i == 0 || i == w) && !corner);
            se_mem[BG_1SB][x + i + (k + y) * 32] =
                ((MASK_BOOL(corner) & MENU_BOX_CORNER) |
                 (MASK_BOOL(horz) & MENU_BOX_EDGE_H) |
                 (MASK_BOOL(vert) & MENU_BOX_EDGE_V) |
                 (MASK_BOOL(i == w) & SE_HFLIP) |
                 (MASK_BOOL(k == h) & SE_VFLIP) |
                 (MASK_BOOL(!corner && !horz && !vert) & BLANK_TILE));
        }
    }
}

// calculate what dimensions to make width and height, given the longest string.
void calcMenuDimensions()
{
    int maxW = -1;
    for (int i = 0; i < menu->nodesLength; i++)
    {
        int strlength = strlen(menu->nodes[i].label) + strlen(menu->nodes[i].data);
        maxW = strlength > maxW ? strlength : maxW;
    }
    menu->width = maxW + 4;
    menu->height = menu->nodesLength * 2 + 1;
}


void initTitleMaster(int initialState) 
{
    initTitleScreen(initialState);
    initTitleDisplay();
    initTitleScreenGFX();
    drawMenu();
    REG_DISPCNT = DCNT_MODE1 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_2D;
}