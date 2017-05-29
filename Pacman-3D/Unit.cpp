#include "Unit.h"



Unit::Unit() {
}


Unit::~Unit() {
}

void Unit::init() {

}

void Unit::update(float deltaTime) {
	if (path.size() > 0) {
		Node *currentWayPoint = path[0];
		targetIndex = 0;
		
		if (Vector3::distance(*position, *currentWayPoint->worldPosition) <= 0.05f) {
			targetIndex++;
			if (targetIndex >= path.size()) {
				
			}
			currentWayPoint = path[targetIndex];
			*position = Vector3::moveTowards(*position, *currentWayPoint->worldPosition, speed * deltaTime);
		}
	}
}