#pragma once
#include "SDL.h"
#include "AssetLoader.h"
#include "Vector2.h"

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

	// Position - x, y
	Vector2 render_position_;
	// Size - width, height
	Vector2 render_size_;

	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) = 0;
};
