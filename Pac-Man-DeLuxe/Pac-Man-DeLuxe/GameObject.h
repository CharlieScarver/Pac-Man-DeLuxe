#pragma once
#include "SDL.h"
#include "AssetLoader.h"
#include "Vector2.h"
#include "Vector2F.h"

class GameObject {
private:
	/// <summary>
	/// The unique ID assigned to the last created instance.
	/// </summary>
	static int current_id_;

protected:
	GameObject(float x, float y, float width, float height);
	GameObject(float x, float y, float width, float height, int spritesheet_x, int spritesheet_y, int sprite_width, int sprite_height);
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
	/// Relative to the top-left vertex of the game window.
	/// </summary>
	Vector2F render_position_;

	// Size - width, height
	/// <summary>
	/// The size (width, height) that the GameObject's sprite will be rendered in.
	/// </summary>
	Vector2F render_size_;

	/// <summary>
	/// The position (x, y) of the sprite on the spritesheet.
	/// Represents the top-left vertex if the sprite.
	/// Relative to the top-left vertex of the spritesheet.
	/// </summary>
	Vector2 spritesheet_position_;

	/// <summary>
	/// The size (width, height) of the sprite on the spritesheet.
	/// </summary>
	Vector2 sprite_size_;

	/// <summary>
	/// Renders the GameObject on the screen using the given rendered.
	/// </summary>
	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) = 0;
};
