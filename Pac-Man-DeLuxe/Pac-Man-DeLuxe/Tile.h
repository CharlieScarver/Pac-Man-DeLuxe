#pragma once
#include "GameObject.h"

class Tile : GameObject {
protected:
	int sprite_sheet_x_;
	int sprite_sheet_y_;

public:
	bool is_solid;

	Tile(float x, float y, float width, float height, int sprite_sheet_x, int sprite_sheet_y);
	~Tile();

	void update() override;
	void render(SDL_Renderer* renderer) override;
};

