#include "GameObject.h"

int GameObject::current_id_ = 0;

GameObject::GameObject(float draw_x, float draw_y, float width, float height) {
	this->id_ = ++current_id_;
	this->render_x_ = draw_x;
	this->render_y_ = draw_y;
	this->render_width_ = width;
	this->render_height_ = height;
}

GameObject::~GameObject() {}
