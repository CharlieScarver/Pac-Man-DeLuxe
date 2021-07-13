#pragma once
#include "GameObject.h"

#define TILE_SPRITE_WIDTH 8
#define TILE_SPRITE_HEIGHT 8

// Determines the scale of the tiles (has to be a multiple of TILE_SPRITE_WIDTH)
#define TILE_WIDTH 24
#define TILE_HEIGHT 24

typedef enum {
	EMPTY,
	WALL_HORIZONTAL_TOP,
	WALL_HORIZONTAL_BOTTOM,
	WALL_VERTICAL_LEFT,
	WALL_VERTICAL_RIGHT,
	WALL_ROUNDED_TOP_LEFT,
	WALL_ROUNDED_TOP_RIGHT,
	WALL_ROUNDED_BOTTOM_LEFT,
	WALL_ROUNDED_BOTTOM_RIGHT
} TileType;

class Tile final : public GameObject {
private:
	int sprite_sheet_x_;
	int sprite_sheet_y_;

public:
	// Tile properties
	TileType type_;
	bool is_solid;
	// Column = width
	int map_x_;
	// Row = height
	int map_y_;

	// Tile contents
	bool contains_player;
	bool contains_ghost;
	// Item* contained_item;

	Tile(int x, int y, TileType type);
	~Tile();

	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};

