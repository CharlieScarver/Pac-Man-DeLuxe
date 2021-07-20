#include "GameObject.h"

int GameObject::current_id_ = 0;

GameObject::GameObject(float render_x, float render_y, float width, float height)
	: render_position_(render_x, render_y), render_size_(width, height)
{
	this->id_ = ++current_id_;
}

GameObject::~GameObject() {}
