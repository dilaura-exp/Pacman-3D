#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include <SDL\SDL_mixer.h>
#include "Pathfinding.h"
#include "IOManager.h"
#include "Pacman.h"
#include "Ghost.h"

enum class GameState{PLAY, GAME_OVER, EXIT};

class Grid;

class MainGame {
public:

	static MainGame &getInstance();
	~MainGame();

	void run();

private:
	SDL_Window* window;
	int screenWidth;
	int screenHeight;
	GameState gameState;

	Uint32 oldTime, currentTime;
	float deltaTime;

	Grid *grid;
	Pacman *pacman;
	std::vector<Ghost*> ghosts;

	void initSystems();
	void initGameObjects();
	void gameLoop();
	void processInput();
	void drawGame();
	void blinkingIntro();

	Mix_Chunk *introSound;
};

