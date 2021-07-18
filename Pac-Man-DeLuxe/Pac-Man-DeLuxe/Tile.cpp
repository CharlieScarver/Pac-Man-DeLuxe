#include "Tile.h"

Tile::Tile(int x, int y, TileType type, bool is_turn_tile = false)
	: GameObject(x * TILE_RENDER_WIDTH, y * TILE_RENDER_HEIGHT, TILE_RENDER_WIDTH, TILE_RENDER_HEIGHT) {
	this->type_ = type;
    this->map_x_ = x;
    this->map_y_ = y;
    this->contains_player_ = false;
    this->contains_ghost_ = false;
    this->is_turn_tile_ = is_turn_tile;

	switch (this->type_)
	{
        case EMPTY:
            // Empty tiles doesn't have a sprite and are not rendered
            // They receive sprite sheet coordinates (0,0) as the defaults
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 0;
            this->is_solid_ = false;
            break;
        case WALL_HORIZONTAL_TOP:
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case WALL_HORIZONTAL_BOTTOM:
            this->spritesheet_x_ = 8;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case WALL_VERTICAL_LEFT:
            this->spritesheet_x_ = 16;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case WALL_VERTICAL_RIGHT:
            this->spritesheet_x_ = 24;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case WALL_ROUNDED_TOP_LEFT:
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case WALL_ROUNDED_TOP_RIGHT:
            this->spritesheet_x_ = 8;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case WALL_ROUNDED_BOTTOM_LEFT:
            this->spritesheet_x_ = 16;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case WALL_ROUNDED_BOTTOM_RIGHT:
            this->spritesheet_x_ = 24;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
	}
}


Tile::Tile(int x, int y, TileType type) : Tile(x, y, type, false) {}

Tile::~Tile() {}

void Tile::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// Rendering functions updates a backbuffer, instead of  the screen directly

    SDL_Rect spritesheet_rect;
    spritesheet_rect.x = this->spritesheet_x_;
    spritesheet_rect.y = this->spritesheet_y_;
    spritesheet_rect.w = TILE_SPRITE_WIDTH;
    spritesheet_rect.h = TILE_SPRITE_HEIGHT;

    SDL_FRect render_rect;
    render_rect.x = this->render_x_;
    render_rect.y = this->render_y_;
    render_rect.w = this->render_width_;
    render_rect.h = this->render_height_;

    if (RENDER_TILES_DEBUG) {
        SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);

        SDL_RenderDrawRectF(renderer, &render_rect);
    }

    // Don't render empty tiles
    if (this->type_ == EMPTY) {
        return;
    }

    SDL_RenderCopyF(renderer, asset_loader->tiles_spritesheet_, &spritesheet_rect, &render_rect);
}
