#include "Node.h"


Node::Node(bool walkable, Vector3 *worldPosition, int gridX, int gridY, int gridZ) {
	this->walkable = walkable;
	this->worldPosition = worldPosition;
	this->gridX = gridX;
	this->gridY = gridY;
	this->gridZ = gridZ;
}

Node::~Node() {

}

int Node::getFCost() {
	return gCost + hCost;
}

int Node::getHeapIndex() {
	return heapIndex;
}

void Node::setHeapIndex(int value) {
	heapIndex = value;
}

int Node::CompareTo(Node *node) {
	int compare = 0;
	if (getFCost() < node->getFCost()) {
		compare = -1;
	}
	else if (getFCost() > node->getFCost()) {
		compare = 1;
	}
	if (compare == 0) {
		if (hCost < node->hCost) {
			compare = -1;
		}
		else if (hCost > node->hCost) {
			compare = 1;
		}
	}
	return -1 * compare;
}