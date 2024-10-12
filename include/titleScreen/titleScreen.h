#ifndef TITLESCREEN_H
#define TITLESCREEN_H
#include "tonc.h"

#define MENU_INPUT_STATE 0

#define EMPTY_TYPE 0
#define SEED_TYPE 1
#define WORLD_TYPE 2

typedef int (*actionptr)();

typedef struct MenuNode
{
    char *label;
    char *data; //string that is able to be changed...
    actionptr action;
} MenuNode;


typedef struct Menu
{
    int ctx;
    int cursor;
    int width;
    int height;
    int x;
    int y;
    int nodesLength;
    MenuNode *nodes;
} Menu;

extern Menu *menu;
extern int titleInputState;
extern OBJ_ATTR *titleCursor_OBJ;

extern bool launch;

void initTitleDisplay();
void initTitleScreenGFX();
void updateTitleScreenGFX();
void initTitleObjects();

void initTitleScreen(int initialState);
bool inputTitle();
int inputTitleScreen();
void drawLogo();
void drawMenu();
void eraseBox(int x, int y, int w, int h);
void drawMenuBox(int x, int y, int width, int height);
void drawMenuString(int x, int y, const char *string);
void drawMenuCursor(int x, int y);
void drawKeyboard();
void calcMenuDimensions();

void initTitleMaster();

#endif