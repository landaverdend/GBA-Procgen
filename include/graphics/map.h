#ifndef MAP_H
#define MAP_H
#include "tonc.h"

#define QF_GRAY 1
#define QF_GREEN 2
#define QF_BROWN 3


#define MAP_INPUT 1
#define MIN_ZOOM float2fx(0.150f)
#define MAX_ZOOM float2fx(0.80f)

#define BACK_TO_GAME 0

void initMapMaster();
void initMapDisplay();
void initMapGFX();
void initAffine();
void loadGTMap();

void mapLoop();

int getMapTile(int x, int y);
int getQFTile(int x, int y);
int gtToMap(int gt);

int sfToMap(int sf);
int constructTileWord(int t1, int t2, int t3, int t4);



#endif