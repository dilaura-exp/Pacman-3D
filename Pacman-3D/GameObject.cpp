#include "GameObject.h"


GameObject::GameObject() {
	position = new Vector3();
	rotation = new Vector3();
	scale = new Vector3(1.0f, 1.0f, 1.0f);
}


GameObject::~GameObject() {
}

void GameObject::init() {
	
}

void GameObject::update(float deltaTime) {
	
}

void GameObject::draw() {
	
}

void GameObject::input(SDL_Event &evnt) {

}


void GameObject::onCollision(Collision &collision) {

}

void GameObject::translate(const Vector3 &translation) {
	position->x += translation.x;
	position->y += translation.y;
	position->z += translation.z;
}