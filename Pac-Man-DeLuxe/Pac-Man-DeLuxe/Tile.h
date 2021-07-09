#pragma once
#include "GameObject.h"

typedef enum {
	EMPTY,
	WALL_HORIZONTAL_TOP,
	WALL_HORIZONTAL_BOTTOM,
	WALL_VERTICAL_LEFT,
	WALL_VERTICAL_RIGHT,
	WALL_ROUNDED_TOP_LEFT,
	WALL_ROUNDED_TOP_RIGHT,
	WALL_ROUNDED_BOTTOM_LEFT,
	WALL_ROUNDED_BOTTOM_RIGHT,
	WALL_INNER_CORNER_TOP_LEFT,
	WALL_INNER_CORNER_TOP_RIGHT,
	WALL_INNER_CORDER_BOTTOM_LEFT,
	WALL_INNER_CORNER_BOTTOM_RIGHT
} TileType;

class Tile : GameObject {
protected:
	int sprite_sheet_x_;
	int sprite_sheet_y_;

public:
	bool is_solid;
	TileType type_;

	Tile(float x, float y, float width, float height, TileType type);
	~Tile();

	void update() override;
	void render(SDL_Renderer* renderer) override;
};

