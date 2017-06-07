#pragma once
#include "Unit.h"

enum AIState {
	PATROL, CHASE
};

class Ghost :Unit {
public:
	Ghost();
	~Ghost();

	void init(Grid *grid, float *material);
	void initNode(Node *node);
	void initPatrol(vector<Node*> patrolNodes);
	void draw();
	void update(float deltaTime);
	void setTarget(Pacman *pacman);
	void setTarget(Node *node);

private:
	Grid *grid;
	int model;
	float *material;
	float findPathDelay;
	AIState currentState;
	vector<Node*> patrolNodes;
	int currentPatrolIndex;
	float changeStateDelay;
};

