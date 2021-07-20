#pragma once
#include "GameObject.h"
#include "TileType.h"

#define TILE_SPRITE_WIDTH 8
#define TILE_SPRITE_HEIGHT 8

// Determines the scale of the tiles (has to be a multiple of TILE_SPRITE_WIDTH)
#define TILE_RENDER_WIDTH 24
#define TILE_RENDER_HEIGHT 24

#define RENDER_TILES_DEBUG 1

class Tile final : public GameObject {
private:
	int spritesheet_x_;
	int spritesheet_y_;

public:
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
	bool contains_player_;
	bool contains_ghost_;
	// Item* contained_item;

	Tile(int x, int y, TileType type);
	Tile(int x, int y, TileType type, bool is_turn_tile);
	~Tile();

	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};

