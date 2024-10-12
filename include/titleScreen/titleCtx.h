#ifndef TITLECTX_H
#define TITLECTX_H

#define ROOT_CTX 0
#define PAUSE_CTX 1

#define ROOTCTX_LENGTH 4
#define PAUSE_CTX_LENGTH 2

void setMenuContext();
int sw_rootCtx();
int sw_gameCreationCtx();
int sw_loadGameCtx();
int sw_palEditor();

//actions for menu nodes
int act_switchWT();
int act_openKb();
int act_generate_island();
int act_generate_cave();
int act_resume();
int empty();

#endif
