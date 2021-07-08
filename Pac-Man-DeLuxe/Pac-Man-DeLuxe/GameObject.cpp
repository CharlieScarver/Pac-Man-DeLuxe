#include "GameObject.h"

int GameObject::current_id_ = 0;

GameObject::GameObject(float x, float y, float width, float height) {
	this->id_ = ++current_id_;
	this->x_ = x;
	this->y_ = y;
	this->width_ = width;
	this->height_ = height;
}

GameObject::~GameObject() {}
