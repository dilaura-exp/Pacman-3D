#include "Ghost.h"
#include "ObjectLoader.h"
#include <iostream>

Ghost::Ghost(){
}


Ghost::~Ghost() {

}

void Ghost::init(Grid *grid, float *material) {
	this->grid = grid;
	this->material = material;
	model = ObjectLoader::loadWavefront("ghost.obj");
	findPathDelay = 0.2f;
	targetIndex = 0;
	*scale = Vector3(0.5f, 0.5f, 0.5f);
	speed = 2.0f;
}

void Ghost::initNode(Node *node) {
	currentNode = node;
	*position = *currentNode->worldPosition;
}

void Ghost::draw() {
	glColor4f(material[0], material[1], material[2], material[3]);
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glScalef(scale->x, scale->y, scale->z);
		glCallList(model);
	glPopMatrix();
}

void Ghost::update(float deltaTime) {
	walkPath(deltaTime);
	findPathDelay -= deltaTime;
	if (findPathDelay <= 0) {
		findPathDelay = 0.2f;
		path = Pathfinding::findPath(currentNode, target->getCurrentNode());
		if (path.size() > 0) {
			//currentNode = path[0];
			targetIndex = 0;
		}
	}
}

void Ghost::setTarget(Pacman *pacman) {
	target = pacman;
}

void Ghost::setTarget(Node *node) {

}