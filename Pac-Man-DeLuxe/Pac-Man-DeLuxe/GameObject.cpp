#include "GameObject.h"

int GameObject::current_id_ = 0;

GameObject::GameObject(float render_x, float render_y, float width, float height)
	: render_position_(render_x, render_y), render_size_(width, height), spritesheet_position_(0, 0), sprite_size_(0, 0)
{
	this->id_ = ++current_id_;
}

GameObject::GameObject(float render_x, float render_y, float width, float height, float spritesheet_x, float spritesheet_y, float sprite_width, float sprite_height)
	: render_position_(render_x, render_y), render_size_(width, height), spritesheet_position_(spritesheet_x, spritesheet_y), sprite_size_(sprite_width, sprite_height)
{
	this->id_ = ++current_id_;
}

GameObject::~GameObject() {}
