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
        case TileType::EMPTY:
            // Empty tiles doesn't have a sprite and are not rendered
            // They receive sprite sheet coordinates (0,0) as the defaults
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 0;
            this->is_solid_ = false;
            break;
        case TileType::WALL_HORIZONTAL_TOP:
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case TileType::WALL_HORIZONTAL_BOTTOM:
            this->spritesheet_x_ = 8;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case TileType::WALL_VERTICAL_LEFT:
            this->spritesheet_x_ = 16;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case TileType::WALL_VERTICAL_RIGHT:
            this->spritesheet_x_ = 24;
            this->spritesheet_y_ = 0;
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_TOP_LEFT:
            this->spritesheet_x_ = 0;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_TOP_RIGHT:
            this->spritesheet_x_ = 8;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_BOTTOM_LEFT:
            this->spritesheet_x_ = 16;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_BOTTOM_RIGHT:
            this->spritesheet_x_ = 24;
            this->spritesheet_y_ = 8;
            this->is_solid_ = true;
            break;
	}
}


Tile::Tile(int x, int y, TileType type) : Tile(x, y, type, false) {}

Tile::~Tile() {}

void Tile::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
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

    if (RENDER_TILES_DEBUG && this->type_ == TileType::EMPTY) {
        // Draw tile box
        SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);
        SDL_RenderDrawRectF(renderer, &render_rect);

        SDL_SetRenderDrawColor(renderer, 75, 75, 0, 0);
        // Draw tile center point axes
        SDL_RenderDrawLineF(
            renderer,
            this->render_x_,
            this->render_y_ + (float)TILE_RENDER_HEIGHT / 2.0f,
            this->render_x_ + TILE_RENDER_WIDTH,
            this->render_y_ + (float)TILE_RENDER_HEIGHT / 2.0f
        );
        SDL_RenderDrawLineF(
            renderer,
            this->render_x_ + (float)TILE_RENDER_WIDTH / 2.0f,
            this->render_y_,
            this->render_x_ + (float)TILE_RENDER_WIDTH / 2.0f,
            this->render_y_ + TILE_RENDER_HEIGHT
        );
    }

    // Don't render empty tiles
    if (this->type_ == TileType::EMPTY) {
        return;
    }

    // Render the tile
    SDL_RenderCopyF(renderer, asset_loader->tiles_spritesheet_, &spritesheet_rect, &render_rect);
}
