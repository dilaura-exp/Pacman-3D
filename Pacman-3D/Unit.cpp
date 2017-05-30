#include "Unit.h"
#include <iostream>


Unit::Unit() {
}


Unit::~Unit() {
}

void Unit::init() {

}

void Unit::walkPath(float deltaTime) {
	if (path.size() > 0) {
		if (*position == *currentNode->worldPosition && targetIndex <= path.size()) {
			targetIndex++;
			if (targetIndex < path.size()) {
				currentNode = path[targetIndex];
			}
		}

		if (targetIndex < path.size()) {
			*position = Vector3::moveTowards(*position, *currentNode->worldPosition, speed * deltaTime);
		}


		/*if (position->x < currentNode->worldPosition->x) {
			translate(Vector3(speed * deltaTime, 0, 0));
		}
		else if (position->x > currentNode->worldPosition->x) {
			translate(Vector3(-speed * deltaTime, 0, 0));
		}
		if (position->z < currentNode->worldPosition->z) {
			translate(Vector3(0, 0, speed * deltaTime));
		}
		else if (position->z > currentNode->worldPosition->z) {
			translate(Vector3(0, 0, -speed * deltaTime));
		}*/
	}
}