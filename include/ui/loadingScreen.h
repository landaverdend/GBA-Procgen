#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

void startLoadScreen(int gt, int bar, int stringID);
void endLoadScreen();

void loadString(int stringID, int block);

void progressMade();
void progressNode();
void setProgress(int x);

#endif