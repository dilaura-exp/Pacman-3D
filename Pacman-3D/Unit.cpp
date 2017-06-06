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
			//targetIndex++;
			if (targetIndex < path.size()) {
				currentNode = path[targetIndex];
			}
		}

		if (path.size() > 1) {
			*position = Vector3::moveTowards(*position, *currentNode->worldPosition, speed * deltaTime);
		}
		else if (path.size() == 1) {
			*position = Vector3::moveTowards(*position, *target->position, speed * deltaTime);
		}
	}
}