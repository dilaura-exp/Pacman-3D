#include "MainGame.h"
#include "ErrorLog.h"
#include "ImageLoader.h"
#include "ObjectLoader.h"
#include "GLTexture.h"

#include <iostream>
#include <vector>

MainGame::MainGame() {
	screenWidth = 1366;
	screenHeight = 768;
	window = nullptr;
	gameState = GameState::PLAY;
	currentTime = SDL_GetTicks();
	deltaTime = 0;
}

MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	initGameObjects();
	gameLoop();
}

GLTexture texture;
int cube;

void MainGame::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("FPS Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		logFatalError("SDL Window could not be created.");
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		logFatalError("SDL GL Context could not be created.");
	}
	GLenum glStatus = glewInit();
	if (glStatus != GLEW_OK) {
		logFatalError("glew could not be initialized.");
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0, 0, 0, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)screenWidth / (GLdouble)screenHeight, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 10.0);
	float color[] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, color);
	
	//glEnable(GL_COLOR_MATERIAL);
	
	//glEnable(GL_TEXTURE_2D);
	//texture = ImageLoader::loadPNG("brick3.png");

	/*float dif[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	float amb[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);*/
}

void MainGame::initGameObjects() {
	grid = new Grid();
	grid->init(0.5f);
	Pathfinding::getInstance().init(grid);
	pacman = new Pacman();
	pacman->init(grid);

	float **materials = new float*[3];
	materials[0] = new float[4]{ 0.8f, 0.5f, 0.5f, 1 };
	materials[1] = new float[4]{ 0.5f, 0.8f, 0.5f, 1 };
	materials[2] = new float[4]{ 0.8f, 0.5f, 0.8f, 1 };

	for (int i = 0; i < 3; i++) {
		ghosts.push_back(new Ghost());
		ghosts[i]->init(grid, materials[i]);
		ghosts[i]->setTarget(pacman);
	}
	ghosts[0]->initNode(grid->getNodes()[5][8][5]);
	ghosts[1]->initNode(grid->getNodes()[4][8][6]);
	ghosts[2]->initNode(grid->getNodes()[6][8][6]);
}

void MainGame::gameLoop() {
	while (gameState != GameState::EXIT) {
		oldTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - oldTime) / 1000.0f;

		processInput();
		pacman->update(deltaTime);
		for (int i = 0; i < ghosts.size(); i++) {
			ghosts[i]->update(deltaTime);
		}
		
		drawGame();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		if (evnt.type == SDL_QUIT) {
			gameState = GameState::EXIT;
		}
		pacman->input(evnt);
	}
}

float angle = 0.0f;

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float pos[] = { -2.0, 20.0, -3.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glTranslatef(-7, -5, -20);
	glRotatef(45, 1, 0, 0);
	glRotatef(45, 0, 1, 0);

	grid->draw(8);
	pacman->draw();
	for (int i = 0; i < ghosts.size(); i++) {
		ghosts[i]->draw();
	}

	SDL_GL_SwapWindow(window);
}