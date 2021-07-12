#pragma once
#include "SDL.h"
#include "AssetLoader.h"

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
	// IRenderable?
	float render_x_;
	float render_y_;
	float render_width_;
	float render_height_;

	virtual void render(SDL_Renderer* renderer, AssetLoader* asset_loader) = 0;
};
