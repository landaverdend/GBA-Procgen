#include "titleCtx.h"
#include "keyboard.h"
#include <string.h>
#include "titleScreen.h"
#include "tsSheet.h"
#include "worldGeneration.h"
#include "world.h"
#include "graphics.h"
#include "overworldGeneration.h"
#include "caveGeneration.h"
#include "map.h"

MenuNode rootContext[ROOTCTX_LENGTH] = 
{
    {
        .label = "SEED ",
        .data = "              ",
        .action = act_openKb
    },
    {
        .label = "WORLD TYPE   ",
        .data = "SQUARE",
        .action = act_switchWT
    },
    {
        .label = "GENERATE ISLAND",
        .data = "",
        .action = act_generate_island
    },
    {
        .label = "GENERATE CAVE",
        .data = "",
        .action = act_generate_cave
    },
};

MenuNode pauseContext[PAUSE_CTX_LENGTH] = 
{
    {
        .label = "RESUME",
        .data = "",
        .action = act_resume
    },
    {
        .label = "EXIT",
        .data = "",
        .action = empty
    }
};

//change menu state and dimensions.
void setMenuContext(int newctx)
{
    eraseBox(menu->x, menu->y, menu->width, menu->height);
    switch (newctx)
    {
    default:
    case ROOT_CTX:
        menu->nodes = rootContext;
        menu->nodesLength = ROOTCTX_LENGTH;
        menu->x = 3;
        menu->y = 7;
        break;
    case PAUSE_CTX:
        menu->nodes = pauseContext;
        menu->nodesLength = PAUSE_CTX_LENGTH;
        menu->x = 5;
        menu->y = 3;
        break;
    }

    menu->ctx = newctx;
    menu->cursor = 0;
    calcMenuDimensions();
}

int sw_rootCtx()
{
    setMenuContext(ROOT_CTX);
    return MENU_INPUT_STATE;
}

int act_openKb()
{
    enableKeyboard();
    return KEYBOARD_INPUT_STATE;
}

int act_switchWT()
{
    if (!strcmp(menu->nodes[1].data, "CIRCLE")) 
    {
        menu->nodes[1].data = "SQUARE";
        MAP_SHAPE = MAP_SQUARE; 
    }
    else
    {
        MAP_SHAPE = MAP_CIRCLE;
        menu->nodes[1].data = "CIRCLE";
    }
    return MENU_INPUT_STATE;
}

int act_generate_island()
{
    setWorldSeed(menu->nodes[0].data);

    generateOverworld();

    mapLoop();

    //below is for returning to menu from prior state.
    noDisplay();
    initTitleDisplay();
    initTitleScreenGFX();
    drawMenu();
    REG_DISPCNT = DCNT_MODE1 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_2D;

    return MENU_INPUT_STATE;
}

//just a debug action so that we can view maps from menu without having to go through inventory.
int act_generate_cave()
{
    setWorldSeed(menu->nodes[0].data);

    generateCave();

    mapLoop();
    
    //below is for returning to menu from prior state.
    noDisplay();
    initTitleDisplay();
    initTitleScreenGFX();
    drawMenu();
    REG_DISPCNT = DCNT_MODE1 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_2D;

    return MENU_INPUT_STATE;
}

int act_resume()
{
    launch = true;
    return MENU_INPUT_STATE;
}

int act_saveAndExit()
{
    sw_rootCtx();
    launch = false;
    return MENU_INPUT_STATE;
}

int empty() { return MENU_INPUT_STATE; }