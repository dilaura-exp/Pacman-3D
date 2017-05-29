#include "ErrorLog.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

void logFatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	system("PAUSE");
	SDL_Quit();
	exit(69);
}