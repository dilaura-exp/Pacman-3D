#pragma once
#include "Unit.h"


class Ghost :Unit {
public:
	Ghost();
	~Ghost();

	void init(Grid *grid, float *material);
	void initNode(Node *node);
	void draw();
	void update(float deltaTime);
	void setTarget(Pacman *pacman);
	void setTarget(Node *node);

private:
	Grid *grid;
	int model;
	float *material;
	float findPathDelay;
};

