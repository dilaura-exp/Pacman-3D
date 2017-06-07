#include "Ghost.h"
#include "ObjectLoader.h"
#include <iostream>

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

Ghost::Ghost(){
}


Ghost::~Ghost() {

}

void Ghost::init(Grid *grid, float *material) {
	this->grid = grid;
	this->material = material;
	model = ObjectLoader::loadWavefront("Models/ghost.obj");
	findPathDelay = 0.2f;
	targetIndex = 0;
	*scale = Vector3(0.45f, 0.45f, 0.45f);
	speed = 2.0f;
}

void Ghost::initNode(Node *node) {
	currentNode = node;
	*position = *currentNode->worldPosition;
}

void Ghost::initPatrol(vector<Node*> patrolNodes) {
	this->patrolNodes = patrolNodes;
	currentPatrolIndex = 0;
	currentState = AIState::PATROL;
	changeStateDelay = randomFloat(10.0f, 20.0f);
}

void Ghost::draw() {
	if (currentNode->gridY > grid->getCurrentHeight()) {
		material[3] = 0.15f;
	}
	else {
		material[3] = 1.0f;
	}
	glColor4f(material[0], material[1], material[2], material[3]);
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glScalef(scale->x, scale->y, scale->z);
		glCallList(model);
	glPopMatrix();
}

void Ghost::update(float deltaTime) {
	path.size();
	findPathDelay -= deltaTime;
	changeStateDelay -= deltaTime;
	if (changeStateDelay <= 0) {
		if (currentState == AIState::PATROL) {
			currentState = AIState::CHASE;
			changeStateDelay = randomFloat(5.0f, 10.0f);
		}
		else if (currentState == AIState::CHASE) {
			currentState = AIState::PATROL;
			changeStateDelay = randomFloat(10.0f, 20.0f);
		}
	}
	if (findPathDelay <= 0) {
		findPathDelay = 0.2f;
		if (currentState == AIState::PATROL) {
			path = Pathfinding::findPath(currentNode, patrolNodes[currentPatrolIndex]);
			if (path.size() > 0) {
				//currentNode = path[0];
				targetIndex = 0;
			}
		}
		else if (currentState == AIState::CHASE) {
			cout << "CHASE" << endl;
			path = Pathfinding::findPath(currentNode, target->getCurrentNode());
			if (path.size() > 0) {
				//currentNode = path[0];
				targetIndex = 0;
			}
		}
	}

	if (path.size() > 1) {
		walkPath(deltaTime);
	}
	else {
		if (currentState == AIState::PATROL) {
			*position = Vector3::moveTowards(*position, *patrolNodes[currentPatrolIndex]->worldPosition, speed * deltaTime);
			if (Vector3::distance(*position, *patrolNodes[currentPatrolIndex]->worldPosition) < 0.05f) {
				currentNode = patrolNodes[currentPatrolIndex];
				currentPatrolIndex++;
				if (currentPatrolIndex >= patrolNodes.size()) {
					currentPatrolIndex = 0;
				}
				path = Pathfinding::findPath(currentNode, patrolNodes[currentPatrolIndex]);
				if (path.size() > 0) {
					//currentNode = path[0];
					targetIndex = 0;
				}
			}
		} else if (currentState == AIState::CHASE) {
			*position = Vector3::moveTowards(*position, *target->position, speed * deltaTime);
		}
	}

	if (Vector3::distance(*position, *target->position) < 0.05f) {
		target->die();
	}
}

void Ghost::setTarget(Pacman *pacman) {
	target = pacman;
}

void Ghost::setTarget(Node *node) {

}