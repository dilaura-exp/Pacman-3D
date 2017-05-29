#pragma once
#include "GameObject.h"

class Node {
public:
	Node(bool walkable, Vector3 *worldPosition, int gridX, int gridY, int gridZ);
	~Node();

	int getFCost();
	int getHeapIndex();
	void setHeapIndex(int value);
	int CompareTo(Node *node);

	bool walkable;
	Vector3 *worldPosition;

	int gridX, gridY, gridZ;
	int gCost, hCost;

	Node *parent;

private:
	int heapIndex;
};

