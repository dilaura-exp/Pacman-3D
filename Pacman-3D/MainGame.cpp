#include "MainGame.h"
#include "ErrorLog.h"
#include "ImageLoader.h"
#include "ObjectLoader.h"
#include "GLTexture.h"

#include <iostream>
#include <vector>

MainGame &MainGame::getInstance() {
	static MainGame instance;
	return instance;
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
	screenWidth = 1366;
	screenHeight = 768;
	window = nullptr;
	gameState = GameState::PLAY;
	currentTime = SDL_GetTicks();
	deltaTime = 0;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("FPS Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		logFatalError("SDL Window could not be created.");
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		logFatalError("SDL GL Context could not be created.");
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { 
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError()); 
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
	introSound = Mix_LoadWAV("Audios/pacman_beginning.wav");
	grid = new Grid();
	grid->init(0.5f);
	Pathfinding::getInstance().init(grid);
	pacman = new Pacman();
	pacman->init(grid);

	float **materials = new float*[6];
	materials[0] = new float[4]{ 0.8f, 0.5f, 0.5f, 1 };
	materials[1] = new float[4]{ 0.5f, 0.8f, 0.5f, 1 };
	materials[2] = new float[4]{ 0.8f, 0.5f, 0.8f, 1 };
	materials[3] = new float[4]{ 0.25f, 0.75f, 0.25f, 1 };
	materials[4] = new float[4]{ 0.75f, 0.25f, 0.25f, 1 };
	materials[5] = new float[4]{ 0.25f, 0.25f, 0.75f, 1 };

	for (int i = 0; i < 6; i++) {
		ghosts.push_back(new Ghost());
		ghosts[i]->init(grid, materials[i]);
		ghosts[i]->setTarget(pacman);
	}
	ghosts[0]->initNode(grid->getNodes()[5][8][5]);
	ghosts[1]->initNode(grid->getNodes()[4][8][5]);
	ghosts[2]->initNode(grid->getNodes()[6][8][5]);
	ghosts[3]->initNode(grid->getNodes()[5][8][6]);
	ghosts[4]->initNode(grid->getNodes()[4][8][6]);
	ghosts[5]->initNode(grid->getNodes()[6][8][6]);

	ghosts[0]->initPatrol(vector<Node*> {
		grid->getNodes()[0][8][0],
		grid->getNodes()[4][8][0],
		grid->getNodes()[4][8][3],
		grid->getNodes()[2][8][8],
		grid->getNodes()[5][8][8],
		grid->getNodes()[5][8][10],
		grid->getNodes()[0][8][10],
	});
	ghosts[1]->initPatrol(vector<Node*> {
		grid->getNodes()[10][8][0],
		grid->getNodes()[6][8][0],
		grid->getNodes()[6][8][3],
		grid->getNodes()[8][8][8],
		grid->getNodes()[5][8][8],
		grid->getNodes()[5][8][10],
		grid->getNodes()[10][8][10],
	});
	ghosts[2]->initPatrol(vector<Node*> {
		grid->getNodes()[0][4][1],
		grid->getNodes()[0][4][10],
		grid->getNodes()[2][4][10],
		grid->getNodes()[2][4][1],
		grid->getNodes()[4][4][1],
		grid->getNodes()[4][4][10],
	});
	ghosts[3]->initPatrol(vector<Node*> {
		grid->getNodes()[10][4][1],
		grid->getNodes()[10][4][10],
		grid->getNodes()[8][4][10],
		grid->getNodes()[8][4][1],
		grid->getNodes()[6][4][1],
		grid->getNodes()[6][4][10],
	});
	ghosts[4]->initPatrol(vector<Node*> {
		grid->getNodes()[0][0][1],
			grid->getNodes()[0][0][10],
			grid->getNodes()[2][0][10],
			grid->getNodes()[4][0][2],
	});
	ghosts[5]->initPatrol(vector<Node*> {
		grid->getNodes()[10][0][1],
			grid->getNodes()[10][0][10],
			grid->getNodes()[8][0][10],
			grid->getNodes()[6][0][2],
	});
}

void MainGame::gameLoop() {
	drawGame();
	Mix_PlayChannel(-1, introSound, 0);
	SDL_Delay(4000);
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

	for (int i = 0; i < ghosts.size(); i++) {
		ghosts[i]->draw();
	}
	pacman->draw();
	grid->draw();

	SDL_GL_SwapWindow(window);
}