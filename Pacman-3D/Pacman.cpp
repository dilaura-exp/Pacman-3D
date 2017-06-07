#include "Pacman.h"
#include "ObjectLoader.h"
#include <iostream>
#include "MainGame.h"

using namespace std;


Pacman::Pacman() {
}


Pacman::~Pacman() {
}

void Pacman::init(Grid *grid) {
	this->grid = grid;
	models[0] = ObjectLoader::loadWavefront("Models/pacman1.obj");
	models[1] = ObjectLoader::loadWavefront("Models/pacman2.obj");
	currentModel = 0;
	animationDelay = 0.2f;
	currentNode = grid->getNodes()[5][8][8];
	lastVisitedNode = currentNode;
	*position = *currentNode->worldPosition;
	*scale = Vector3(0.4f, 0.4f, 0.4f);
	speed = 2.0f;
	ladderDirection = 0;
	grid->setCurrentHeight(currentNode->gridY);
	sound = Mix_LoadWAV("Audios/pacman_chomp.wav");
	deathSound = Mix_LoadWAV("Audios/pacman_death.wav");
	if (sound == NULL) { 
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError()); 
	}
}

void Pacman::update(float deltaTime) {
	//cout << currentNode->worldPosition->x << " " << currentNode->worldPosition->y << " " << currentNode->worldPosition->z << endl;
	animationDelay -= deltaTime;
	if (animationDelay <= 0) {
		animationDelay = 0.2f;
		currentModel = 1 - currentModel;
	}
	move(deltaTime);
}

void Pacman::draw() {
	float material[]{ 0.75, 0.75, 0.25, 1 };
	glColor4f(material[0], material[1], material[2], material[3]);
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glRotatef(rotation->x, 1, 0, 0);
		glRotatef(rotation->y, 0, 1, 0);
		glRotatef(rotation->z, 0, 0, 1);
		glScalef(scale->x, scale->y, scale->z);
		glCallList(models[currentModel]);
	glPopMatrix();
}

void Pacman::input(SDL_Event &evnt) {
	if (evnt.type == SDL_KEYDOWN) {
		if (evnt.key.keysym.sym == SDLK_LEFT) {
			nextDirection = WalkDirection::LEFT;
		}
		if (evnt.key.keysym.sym == SDLK_RIGHT) {
			nextDirection = WalkDirection::RIGHT;
		}
		if (evnt.key.keysym.sym == SDLK_UP) {
			nextDirection = WalkDirection::UP;
		}
		if (evnt.key.keysym.sym == SDLK_DOWN) {
			nextDirection = WalkDirection::DOWN;
		}
		if (evnt.key.keysym.sym == SDLK_SPACE) {
			ladderInput = true;
		}
	}
	if (evnt.type == SDL_KEYUP) {
		if (evnt.key.keysym.sym == SDLK_SPACE) {
			ladderInput = false;
		}
	}
}

void Pacman::move(float deltaTime) {
	if (ladderDirection == 1) {
		if (currentNode->gridY < grid->getSizeY() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY + 1][currentNode->gridZ];
			if (targetNode->walkable) {
				translate(Vector3(0, speed*deltaTime, 0));
				if (position->y >= targetNode->worldPosition->y) {
					currentNode = targetNode;
					grid->setCurrentHeight(currentNode->gridY);
				}
				return;
			}
		}
		ladderDirection = 0;
	}
	else if (ladderDirection == -1) {
		if (currentNode->gridY > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY - 1][currentNode->gridZ];
			if (targetNode->walkable) {
				translate(Vector3(0, -speed*deltaTime, 0));
				if (position->y < targetNode->worldPosition->y) {
					currentNode = targetNode;
					grid->setCurrentHeight(currentNode->gridY);
				}
				return;
			}
		}
		ladderDirection = 0;
	}

	if (ladderInput) {
		if (currentNode->gridY < grid->getSizeY() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY + 1][currentNode->gridZ];
			if (targetNode->walkable) {
				ladderDirection = 1;
			}
		}
		if (currentNode->gridY > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY - 1][currentNode->gridZ];
			if (targetNode->walkable) {
				ladderDirection = -1;
			}
		}
	}
	if (walkDirection == WalkDirection::STAY) {
		walkDirection = nextDirection;
	}

	if (nextDirection == WalkDirection::RIGHT) {
		if (currentNode->gridX < grid->getSizeX() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX + 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				if (lastVisitedNode != currentNode) {
					lastVisitedNode = currentNode;
					walkDirection = nextDirection;
				}
			}
		}
	}
	else if (nextDirection == WalkDirection::LEFT) {
		if (currentNode->gridX > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX - 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				if (lastVisitedNode != currentNode) {
					lastVisitedNode = currentNode;
					walkDirection = nextDirection;
				}
			}
		}
	}
	else if (nextDirection == WalkDirection::DOWN) {
		if (currentNode->gridZ < grid->getSizeZ() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ + 1];
			if (targetNode->walkable) {
				if (lastVisitedNode != currentNode) {
					lastVisitedNode = currentNode;
					walkDirection = nextDirection;
				}
			}
		}
	}
	else if (nextDirection == WalkDirection::UP) {
		if (currentNode->gridZ > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ - 1];
			if (targetNode->walkable) {
				if (lastVisitedNode != currentNode) {
					lastVisitedNode = currentNode;
					walkDirection = nextDirection;
				}
			}
		}
	}

	if (walkDirection == WalkDirection::RIGHT) {
		if (currentNode->gridX < grid->getSizeX() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX + 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				rotation->y = -90;
				translate(Vector3(speed*deltaTime, 0, 0));
				if (position->x >= targetNode->worldPosition->x) {
					currentNode = targetNode;
					if (currentNode->item == 1) {
						currentNode->item = 0;
						Mix_PlayChannel(-1, sound, 0);
					}
				}
			}
		}
	}
	else if (walkDirection == WalkDirection::LEFT) {
		if (currentNode->gridX > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX - 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				rotation->y = 90;
				translate(Vector3(-speed*deltaTime, 0, 0));
				if (position->x <= targetNode->worldPosition->x) {
					currentNode = targetNode;
					if (currentNode->item == 1) {
						currentNode->item = 0;
						Mix_PlayChannel(-1, sound, 0);
					}
				}
			}
		}
	}
	else if (walkDirection == WalkDirection::DOWN) {
		if (currentNode->gridZ < grid->getSizeZ() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ + 1];
			if (targetNode->walkable) {
				rotation->y = 180;
				translate(Vector3(0, 0, speed*deltaTime));
				if (position->z >= targetNode->worldPosition->z) {
					currentNode = targetNode;
					if (currentNode->item == 1) {
						currentNode->item = 0;
						Mix_PlayChannel(-1, sound, 0);
					}
				}
			}
		}
	}
	else if (walkDirection == WalkDirection::UP) {
		if (currentNode->gridZ > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ - 1];
			if (targetNode->walkable) {
				rotation->y = 0;
				translate(Vector3(0, 0, -speed*deltaTime));
				if (position->z <= targetNode->worldPosition->z) {
					currentNode = targetNode;
					if (currentNode->item == 1) {
						currentNode->item = 0;
						Mix_PlayChannel(-1, sound, 0);
					}
				}
			}
		}
	}
}

void Pacman::die() {
	Mix_PlayChannel(-1, deathSound, 0);
	SDL_Delay(2000);
	(&MainGame::getInstance())->~MainGame();
	MainGame::getInstance().run();
}

Node *Pacman::getCurrentNode() {
	return currentNode;
}