#include <stdlib.h>
#include "main.h"
#include "titleScreen.h"
#include "tsSheet.h"
#include "world.h"
#include "worldGeneration.h"
#include "titleCtx.h"

int main()
{
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	while (1) {		
			menuLoop();
	}
	return 0;
}

void menuLoop() {
	initTitleMaster(ROOT_CTX);
	while (1) 
	{
		VBlankIntrWait();
		if (inputTitle()) 
			break;
		updateTitleScreenGFX();
	}
}