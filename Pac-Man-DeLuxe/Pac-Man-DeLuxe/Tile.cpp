#include "Tile.h"

Tile::Tile(int x, int y, TileType type)
	: GameObject(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT) {
	this->type_ = type;
    this->map_x_ = x;
    this->map_y_ = y;
    this->contains_player = false;
    this->contains_ghost = false;

	switch (this->type_)
	{
        case EMPTY:
            // Empty tiles doesn't have a sprite and are not rendered
            // They receive sprite sheet coordinates (0,0) as the defaults
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = false;
            break;
        case WALL_HORIZONTAL_TOP:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
            break;
        case WALL_HORIZONTAL_BOTTOM:
            this->sprite_sheet_x_ = 8;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
            break;
        case WALL_VERTICAL_LEFT:
            this->sprite_sheet_x_ = 16;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
            break;
        case WALL_VERTICAL_RIGHT:
            this->sprite_sheet_x_ = 24;
            this->sprite_sheet_y_ = 0;
            this->is_solid = true;
            break;
        case WALL_ROUNDED_TOP_LEFT:
            this->sprite_sheet_x_ = 0;
            this->sprite_sheet_y_ = 8;
            this->is_solid = true;
            break;
        case WALL_ROUNDED_TOP_RIGHT:
            this->sprite_sheet_x_ = 8;
            this->sprite_sheet_y_ = 8;
            this->is_solid = true;
            break;
        case WALL_ROUNDED_BOTTOM_LEFT:
            this->sprite_sheet_x_ = 16;
            this->sprite_sheet_y_ = 8;
            this->is_solid = true;
            break;
        case WALL_ROUNDED_BOTTOM_RIGHT:
            this->sprite_sheet_x_ = 24;
            this->sprite_sheet_y_ = 8;
            this->is_solid = true;
            break;
	}
}

Tile::~Tile() {}

void Tile::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// Rendering functions updates a backbuffer, instead of  the screen directly
	SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);

    SDL_Rect spritesheet_rect;
    spritesheet_rect.x = this->sprite_sheet_x_;
    spritesheet_rect.y = this->sprite_sheet_y_;
    spritesheet_rect.w = TILE_SPRITE_WIDTH;
    spritesheet_rect.h = TILE_SPRITE_HEIGHT;

    SDL_FRect render_rect;
    render_rect.x = this->render_x_;
    render_rect.y = this->render_y_;
    render_rect.w = this->render_width_;
    render_rect.h = this->render_height_;

	SDL_RenderDrawRectF(renderer, &render_rect);

    // Don't render empty tiles
    if (this->type_ == EMPTY) {
        return;
    }

    SDL_RenderCopyF(renderer, asset_loader->tiles_spritesheet_, &spritesheet_rect, &render_rect);
}
