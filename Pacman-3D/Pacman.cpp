#include "Pacman.h"
#include "ObjectLoader.h"
#include <iostream>

using namespace std;


Pacman::Pacman() {
}


Pacman::~Pacman() {
}

void Pacman::init(Grid *grid) {
	this->grid = grid;
	models[0] = ObjectLoader::loadWavefront("pacman1.obj");
	models[1] = ObjectLoader::loadWavefront("pacman2.obj");
	currentModel = 0;
	animationDelay = 0.25f;
	currentNode = grid->getNodes()[5][8][8];
	*position = *currentNode->worldPosition;
	*scale = Vector3(0.4f, 0.4f, 0.4f);
	speed = 1.5f;
	ladderDirection = 0;
	grid->setCurrentHeight(currentNode->gridY);
}

void Pacman::update(float deltaTime) {
	animationDelay -= deltaTime;
	if (animationDelay <= 0) {
		animationDelay = 0.25f;
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
			leftInput = -1;
		}
		if (evnt.key.keysym.sym == SDLK_RIGHT) {
			rightInput = 1;
		}
		if (evnt.key.keysym.sym == SDLK_UP) {
			upInput = -1;
		}
		if (evnt.key.keysym.sym == SDLK_DOWN) {
			downInput = 1;
		}
		if (evnt.key.keysym.sym == SDLK_SPACE) {
			ladderInput = true;
		}
	}
	if (evnt.type == SDL_KEYUP) {
		if (evnt.key.keysym.sym == SDLK_LEFT) {
			leftInput = 0;
		}
		if (evnt.key.keysym.sym == SDLK_RIGHT) {
			rightInput = 0;
		}
		if (evnt.key.keysym.sym == SDLK_UP) {
			upInput = 0;
		}
		if (evnt.key.keysym.sym == SDLK_DOWN) {
			downInput = 0;
		}
	}
	horizontalInput = rightInput + leftInput;
	verticalInput = upInput + downInput;
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
			}
			else {
				ladderDirection = 0;
			}
		}
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
			}
			else {
				ladderDirection = 0;
			}
		}
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
		ladderInput = false;
	}
	else if (horizontalInput > 0) {
		if (currentNode->gridX < grid->getSizeX() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX + 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				rotation->y = -90;
				translate(Vector3(speed*deltaTime, 0, 0));
				if (position->x >= targetNode->worldPosition->x) {
					currentNode = targetNode;
				}
			}
		}
	}
	else if (horizontalInput < 0) {
		if (currentNode->gridX > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX - 1][currentNode->gridY][currentNode->gridZ];
			if (targetNode->walkable) {
				rotation->y = 90;
				translate(Vector3(-speed*deltaTime, 0, 0));
				if (position->x <= targetNode->worldPosition->x) {
					currentNode = targetNode;
				}
			}
		}
	}
	else if (verticalInput > 0) {
		if (currentNode->gridZ < grid->getSizeZ() - 1) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ + 1];
			if (targetNode->walkable) {
				rotation->y = 180;
				translate(Vector3(0, 0, speed*deltaTime));
				if (position->z >= targetNode->worldPosition->z) {
					currentNode = targetNode;
				}
			}
		}
	}
	else if (verticalInput < 0) {
		if (currentNode->gridZ > 0) {
			Node *targetNode = grid->getNodes()[currentNode->gridX][currentNode->gridY][currentNode->gridZ - 1];
			if (targetNode->walkable) {
				rotation->y = 0;
				translate(Vector3(0, 0, -speed*deltaTime));
				if (position->z <= targetNode->worldPosition->z) {
					currentNode = targetNode;
				}
			}
		}
	}
}

Node *Pacman::getCurrentNode() {
	return currentNode;
}