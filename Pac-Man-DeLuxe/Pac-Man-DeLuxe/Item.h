#pragma once
#include "GameObject.h"
#include "ItemType.h"

// Forward declaration to resolve circular dependency
class Tile;

class Item : public GameObject {
private:
	// Sizes
	static const int item_sprite_width_ = 8;
	static const int item_sprite_height_ = 8;
	static const int item_render_width_ = 24;
	static const int item_render_height_ = 24;

	// Animation
	static const int pellet_spritesheet_x_ = 0;
	static const int pellet_spritesheet_y_ = 16;
	static const int energizer_spritesheet_x_ = 8;
	static const int energizer_spritesheet_y_ = 16;
	static const int fruit_spritesheet_x_ = 24;
	static const int fruit_spritesheet_y_ = 16;

	static const int default_item_animation_frames = 1;
	static const int default_animation_delay_ = 200;
	static const int energizer_animation_frames_ = 2;
	static const int energizer_animation_delay_ = 200;

	// Score
	static const int pellet_score_ = 10;
	static const int energizer_score_ = 50;
	static const int fruit_score_ = 100;

protected:
	SDL_Texture* spritesheet_texture_;

public:
	/// <summary>
	/// The type of the item.
	/// The three possible values are: Pellet, Energizer and Fruit.
	/// </summary>
	ItemType item_type_;

	/// <summary>
	/// Shows how much score will the player receive when Pac-Man eats this item.
	/// </summary>
	int score_;

	/// <summary>
	/// The tile that contains this item.
	/// </summary>
	Tile* container_tile_;

	Item(int tile_map_x, float tile_map_y, ItemType item_type);
	Item(Tile* container_tile, ItemType item_type);
	~Item();

	void Update(float delta_time, const Uint8* keyboard_state);
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
