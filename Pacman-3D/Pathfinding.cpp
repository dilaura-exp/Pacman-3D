#include "Pathfinding.h"
#include <iostream>

using namespace std;

Pathfinding &Pathfinding::getInstance() {
	static Pathfinding instance;
	return instance;
}

Pathfinding::~Pathfinding() {
}

void Pathfinding::init(Grid *grid) {
	this->grid = grid;
}

vector<Node*> Pathfinding::findPath(Node *startNode, Node *targetNode) {
	bool success = false;
	vector<Node*> wayPoints;

	if (true) {
		NodeHeap openSet(getInstance().grid->getMaxSize());
		unordered_set<Node*> closedSet;

		openSet.add(startNode);
		while (openSet.count() > 0) {
			Node *currentNode = openSet.removeFirstNode();

			closedSet.emplace(currentNode);

			if (currentNode == targetNode) {
				success = true;
				break;
			}

			vector<Node*> neighbours = getInstance().grid->getNeighbours(currentNode);
			for (int i = 0; i < neighbours.size(); i++) {
				unordered_set<Node*>::const_iterator it = closedSet.find(neighbours[i]);
				if (!neighbours[i]->walkable || it != closedSet.end()) {
					continue;
				}

				int newCostToNeighbour = currentNode->gCost + getDistance(currentNode, neighbours[i]);
				if (newCostToNeighbour < neighbours[i]->gCost || !openSet.contains(neighbours[i])) {
					neighbours[i]->gCost = newCostToNeighbour;
					neighbours[i]->hCost = getDistance(neighbours[i], targetNode);
					neighbours[i]->parent = currentNode;
					if (!openSet.contains(neighbours[i])) {
						openSet.add(neighbours[i]);
					}
				}
			}
		}
	}
	if (success) {
		wayPoints = makePath(startNode, targetNode);
	}
	return wayPoints;
}

vector<Node*> Pathfinding::makePath(Node *startNode, Node *targetNode) {
	vector<Node*> path;
	Node *currentNode = targetNode;

	while (currentNode != startNode) {
		path.push_back(currentNode);
		currentNode = currentNode->parent;
	}
	reverse(path.begin(), path.end());
	return path;
}

int Pathfinding::getDistance(Node *nodeA, Node *nodeB) {
	int distanceX = abs(nodeA->gridX - nodeB->gridX);
	int distanceY = abs(nodeA->gridY - nodeB->gridY);

	if (distanceX > distanceY)
		return 14 * distanceY + 10 * (distanceX - distanceY);
	else
		return 14 * distanceX + 10 * (distanceY - distanceX);
}