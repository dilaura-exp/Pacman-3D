#pragma once
#include "GameObject.h"
#include "Pathfinding.h" 
#include <SDL\SDL_mixer.h>

enum WalkDirection {
	STAY = 0, UP, DOWN, LEFT, RIGHT
};

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
	Node *lastVisitedNode;

	float animationDelay;

	WalkDirection walkDirection;
	WalkDirection nextDirection;
	bool ladderInput;
	int ladderDirection;

	Mix_Chunk *sound;
};

