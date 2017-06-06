#include <time.h>
#include <stdlib.h>
#include "MainGame.h"

int main(int argc, char** argv) {

	srand((unsigned)time(NULL));
	MainGame::getInstance().run();
		
	return 0;
}