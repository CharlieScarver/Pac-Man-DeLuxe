#include "Item.h"
#include "Tile.h"

Item::Item(Tile* container_tile, ItemType type) 
	: GameObject(container_tile->render_position_.x_, container_tile->render_position_.y_, Item::item_render_width_, Item::item_render_height_)
{
	this->type_ = type;

	switch (this->type_)
	{
		case ItemType::PELLET:
			this->spritesheet_position_ = Vector2(this->pellet_spritesheet_x_, this->pellet_spritesheet_y_);
			this->score_ = this->pellet_score_;
			break;
		case ItemType::ENERGIZER:
			this->spritesheet_position_ = Vector2(this->energizer_spritesheet_x_, this->energizer_spritesheet_y_);
			this->score_ = this->energizer_score_;
			break;
		case ItemType::FRUIT:
			this->spritesheet_position_ = Vector2(this->fruit_spritesheet_x_, this->fruit_spritesheet_x_);
			this->score_ = this->fruit_score_;
			break;
		default:
			this->spritesheet_position_ = Vector2(0, 0);
			this->score_ = 0;
	}

	this->sprite_size_ = Vector2(this->item_sprite_width_, this->item_sprite_height_);

	this->spritesheet_texture_ = nullptr;
	this->container_tile_ = nullptr;
}

Item::~Item() {}

void Item::Update(float delta_time, const Uint8* keyboard_state) {}

void Item::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
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

    // Render the item
    SDL_RenderCopyF(renderer, asset_loader->tiles_spritesheet_, &spritesheet_rect, &render_rect);
}
