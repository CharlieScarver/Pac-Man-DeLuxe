#include "Tile.h"

Tile::Tile(float x, float y, float width, float height, int sprite_sheet_x, int sprite_sheet_y) 
	: GameObject(x, y, width, height) {
	this->sprite_sheet_x_ = sprite_sheet_x;
	this->sprite_sheet_y_ = sprite_sheet_y;
	this->is_solid = true;
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
