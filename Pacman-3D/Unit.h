#pragma once
#include "GameObject.h"
#include "Pathfinding.h"
#include "Pacman.h"
#include <vector>

using namespace std;

class Unit : public GameObject {
public:
	Unit();
	~Unit();

	void init();
	void walkPath(float deltaTime);

protected:
	Pacman *target;
	float speed;

	Node *currentNode;
	vector<Node*> path;
	int targetIndex;
	
};

