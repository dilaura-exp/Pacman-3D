#pragma once
#include "GameObject.h"
#include "Pathfinding.h"

class Pacman : public GameObject {
public:
	Pacman();
	~Pacman();

	void init(Grid *grid);
	void update(float deltaTime);
	void draw();
	void input(SDL_Event &evnt);

	Node *getCurrentNode();

private:
	void move(float deltaTime);

	Grid *grid;
	float speed;
	int models[2];
	int currentModel;
	Node *currentNode;

	float animationDelay;

	int leftInput, rightInput;
	int downInput, upInput;
	int horizontalInput;
	int verticalInput;
	bool ladderInput;
	int ladderDirection;
};

