#pragma once
#include "GameObject.h"
#include "Pathfinding.h"
#include <vector>

using namespace std;

class Unit : public GameObject {
public:
	Unit();
	~Unit();

	void init();
	void update(float deltaTime);

private:
	GameObject target;
	float speed;

	vector<Node*> path;
	int targetIndex;
	
};

