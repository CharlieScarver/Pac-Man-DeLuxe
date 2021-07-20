#pragma once
#include "SDL.h"
#include "AssetLoader.h"
#include "Vector2.h"

class GameObject {
private:
	/// <summary>
	/// The unique ID assigned to the last created instance.
	/// </summary>
	static int current_id_;

protected:
	GameObject(float x, float y, float width, float height);
	~GameObject();

public:
	/// <summary>
	/// Unique ID of the GameObject instance.
	/// First instance has ID = 1.
	/// </summary>
	int id_;
		
	/// <summary>
	/// The position (x, y) where the GameObject's sprite will be rendered.
	/// Represents the top-left vertex of the rendered sprite.
	/// Relative to the game window. 
	/// </summary>
	Vector2 render_position_;

	// Size - width, height
	/// <summary>
	/// The size (width, height) that the GameObject's sprite will be rendered in.
	/// </summary>
	Vector2 render_size_;

	/// <summary>
	/// Renders the GameObject on the screen using the given rendered.
	/// </summary>
	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) = 0;
};
