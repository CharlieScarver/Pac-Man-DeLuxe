#include "Tile.h"

Tile::Tile(int x, int y, TileType type, Item* contained_item)
	: GameObject(x * Tile::render_width_, y * Tile::render_height_, Tile::render_width_, Tile::render_height_)
{
	this->type_ = type;
    this->map_x_ = x;
    this->map_y_ = y;
    
    this->is_turn_tile_ = false;

    this->sprite_size_ = Vector2(Tile::sprite_width_, Tile::sprite_height_);

    this->contained_item_ = contained_item;

	switch (this->type_)
	{
        case TileType::EMPTY:
            // Empty tiles doesn't have a sprite and are not rendered
            // They receive sprite sheet coordinates (0,0) as the defaults
            this->spritesheet_position_ = Vector2(0, 0);
            this->is_solid_ = false;
            break;
        case TileType::WALL_HORIZONTAL_TOP:
            this->spritesheet_position_ = Vector2(0, 0);
            this->is_solid_ = true;
            break;
        case TileType::WALL_HORIZONTAL_BOTTOM:
            this->spritesheet_position_ = Vector2(8, 0);
            this->is_solid_ = true;
            break;
        case TileType::WALL_VERTICAL_LEFT:
            this->spritesheet_position_ = Vector2(16, 0);
            this->is_solid_ = true;
            break;
        case TileType::WALL_VERTICAL_RIGHT:
            this->spritesheet_position_ = Vector2(24, 0);
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_TOP_LEFT:
            this->spritesheet_position_ = Vector2(0, 8);
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_TOP_RIGHT:
            this->spritesheet_position_ = Vector2(8, 8);
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_BOTTOM_LEFT:
            this->spritesheet_position_ = Vector2(16, 8);
            this->is_solid_ = true;
            break;
        case TileType::WALL_ROUNDED_BOTTOM_RIGHT:
            this->spritesheet_position_ = Vector2(24, 8);
            this->is_solid_ = true;
            break;
	}
}

Tile::Tile(int x, int y, TileType type) : Tile(x, y, type, nullptr) {}

Tile::~Tile() {}

void Tile::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
    // Spritesheet poistion and size actually use integers
    SDL_Rect spritesheet_rect;
    spritesheet_rect.x = (int)this->spritesheet_position_.x_;
    spritesheet_rect.y = (int)this->spritesheet_position_.y_;
    spritesheet_rect.w = (int)this->sprite_size_.x_;
    spritesheet_rect.h = (int)this->sprite_size_.y_;

    SDL_FRect render_rect;
    render_rect.x = this->render_position_.x_;
    render_rect.y = this->render_position_.y_;
    render_rect.w = this->render_size_.x_;
    render_rect.h = this->render_size_.y_;

    if (RENDER_TILES_DEBUG && this->type_ == TileType::EMPTY) {
        // Draw tile box
        SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);
        SDL_RenderDrawRectF(renderer, &render_rect);

        SDL_SetRenderDrawColor(renderer, 75, 75, 0, 0);

        // Draw tile center point axes
        /*SDL_RenderDrawLineF(
            renderer,
            this->render_position_.x_,
            this->render_position_.y_ + (float)TILE_RENDER_HEIGHT / 2.0f,
            this->render_position_.x_ + TILE_RENDER_WIDTH,
            this->render_position_.y_ + (float)TILE_RENDER_HEIGHT / 2.0f
        );
        SDL_RenderDrawLineF(
            renderer,
            this->render_position_.x_ + (float)TILE_RENDER_WIDTH / 2.0f,
            this->render_position_.y_,
            this->render_position_.x_ + (float)TILE_RENDER_WIDTH / 2.0f,
            this->render_position_.y_ + TILE_RENDER_HEIGHT
        );*/
    }

    // Don't render empty tiles
    if (this->type_ != TileType::EMPTY) {
        // Render the tile
        SDL_RenderCopyF(renderer, asset_loader->tiles_spritesheet_, &spritesheet_rect, &render_rect);
    }

    // Render the contained item (if any)
    if (this->contained_item_ != nullptr) {
        this->contained_item_->Render(renderer, asset_loader);
    }
}
