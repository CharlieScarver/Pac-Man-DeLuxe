#include "Tile.h"

Tile::Tile(float x, float y, float width, float height, TileType type)
	: GameObject(x, y, width, height) {
	this->type_ = type;

	switch (this->type_)
	{
        case EMPTY:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = false;
        case WALL_HORIZONTAL_TOP:
            this->sprite_sheet_x_ = 8;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_HORIZONTAL_BOTTOM:
            this->sprite_sheet_x_ = 16;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_VERTICAL_LEFT:
            this->sprite_sheet_x_ = 24;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_VERTICAL_RIGHT:
            this->sprite_sheet_x_ = 32;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_ROUNDED_TOP_LEFT:
            this->sprite_sheet_x_ = 40;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_ROUNDED_TOP_RIGHT:
            this->sprite_sheet_x_ = 48;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_ROUNDED_BOTTOM_LEFT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_ROUNDED_BOTTOM_RIGHT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_INNER_CORNER_TOP_LEFT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_INNER_CORNER_TOP_RIGHT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_INNER_CORDER_BOTTOM_LEFT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
        case WALL_INNER_CORNER_BOTTOM_RIGHT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
	}
}

Tile::~Tile() {}

void Tile::update() {
}
void Tile::render(SDL_Renderer* renderer) {
	// Rendering functions updates a backbuffer, instead of  the screen directly
	SDL_SetRenderDrawColor(renderer, 200, 25, 25, 0);

	SDL_Rect tile_rect = SDL_Rect();
	tile_rect.x = this->x_ * this->width_;
	tile_rect.y = this->y_ * this->height_;
	tile_rect.w = this->width_;
	tile_rect.h = this->height_;

	SDL_RenderDrawRect(renderer, &tile_rect);
}
