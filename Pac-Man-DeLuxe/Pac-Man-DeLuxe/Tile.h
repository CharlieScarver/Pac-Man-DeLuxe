#pragma once
#include "GameObject.h"
#include "TileType.h"
#include "Item.h"

#define RENDER_TILES_DEBUG 0

class Tile final : public GameObject {
public:
	// Sizes
	static const int sprite_width_ = 8;
	static const int sprite_height_ = 8;
	// Has to be a multiple of TILE_SPRITE_WIDTH for integer scaling
	static const int render_width_ = 24;
	static const int render_height_ = 24;

	// Tile properties
	TileType type_;
	// Units cannot enter solid tiles
	bool is_solid_;
	// Turn tiles mark where a unit can make a turn
	bool is_turn_tile_;
	// Column = width
	int map_x_;
	// Row = height
	int map_y_;

	// Tile contents
	Item* contained_item_;

	Tile(int x, int y, TileType type);
	Tile(int x, int y, TileType type, Item* contained_item);
	~Tile();

	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};

