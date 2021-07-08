#pragma once
#include "SDL.h"

class GameObject {
private:
	// The last assigned Unique ID
	static int current_id_;

protected:
	GameObject(float x, float y, float width, float height);
	~GameObject();

public:
	// Unique ID of the GameObject instance (First instance ID = 1)
	int id_;
	float x_;
	float y_;
	float width_;
	float height_;

	virtual void update() = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
};
