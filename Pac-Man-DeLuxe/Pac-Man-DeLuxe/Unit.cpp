#include <iostream>
#include "Unit.h"
#include "Map.h"

Unit::Unit(float x, float y, float width, float height, Map* map) : GameObject(x, y, width, height) {
	// Set defaults
	this->spritesheet_x_ = 0;
	this->spritesheet_y_ = 0;

	this->is_idle_ = true;
	this->is_moving_left_ = false;
	this->is_moving_right_ = false;
	this->is_moving_up_ = false;
	this->is_moving_down_ = false;
	this->is_cornering_ = false;

	this->current_tile_ = nullptr;
	this->current_tile_x_ = 0;
	this->current_tile_y_ = 0;

	this->current_animation_frame_ = 0;
	this->animation_frames_count_ = 1;
	this->animation_delay_ = 0;

	this->velocity_x_ = 0;
	this->velocity_y_ = 0;

	this->map_ = map;

	Tile* current_tile = GetTileForUnitCoordinates(x, y);

	if (!current_tile->is_solid_) {
		// If tile is not solid => change current tile
		this->current_tile_x_ = current_tile->map_x_;
		this->current_tile_y_ = current_tile->map_y_;
	}
}

Unit::~Unit() {}

Tile* Unit::GetTileForUnitCoordinates(float x, float y) {
	// Coordinates of the center point of the sprite
	float center_x = x + (UNIT_RENDER_WIDTH / 2);
	float center_y = y + (UNIT_RENDER_HEIGHT / 2);

	// Calculate which tile that should be in
	int center_tile_x = (int)(center_x / TILE_RENDER_WIDTH);
	int center_tile_y = (int)(center_y / TILE_RENDER_HEIGHT);

	// Get the tile object
	return this->map_->GetTile(center_tile_x, center_tile_y);
}

Tile* Unit::GetTileForCenterUnitCoordinates(float center_x, float center_y) {
	// Calculate which tile that should be in
	int center_tile_x = (int)(center_x / TILE_RENDER_WIDTH);
	int center_tile_y = (int)(center_y / TILE_RENDER_HEIGHT);

	// Get the tile object
	return this->map_->GetTile(center_tile_x, center_tile_y);
}

void Unit::SetCenterToTileCenter(Tile* tile) {
	float tile_center_x = (tile->map_x_ * TILE_RENDER_WIDTH) + (float)TILE_RENDER_WIDTH / 2.0f;
	float tile_center_y = (tile->map_y_* TILE_RENDER_HEIGHT) + (float)TILE_RENDER_HEIGHT / 2.0f;

	this->render_x_ = tile_center_x - (float)UNIT_RENDER_WIDTH / 2.0f;
	this->render_y_ = tile_center_y - (float)UNIT_RENDER_HEIGHT / 2.0f;
}

void Unit::StopMoving() {
	this->is_idle_ = true;
	this->is_moving_up_ = false;
	this->is_moving_down_ = false;
	this->is_moving_left_ = false;
	this->is_moving_right_ = false;
}

Tile* Unit::GetNextTileInDirection(Orientation direction) {
	switch (direction)
	{
		case Orientation::UP:
			return this->map_->GetTile(this->current_tile_x_, this->current_tile_y_ - 1);
		case Orientation::DOWN:
			return this->map_->GetTile(this->current_tile_x_, this->current_tile_y_ + 1);
		case Orientation::LEFT:
			return this->map_->GetTile(this->current_tile_x_ - 1, this->current_tile_y_);
		case Orientation::RIGHT:
			return this->map_->GetTile(this->current_tile_x_ + 1, this->current_tile_y_);
		default:
			return nullptr;
	}
}

void Unit::ManageMovement(float delta_time) {
	// If unit is not moving we can stop here
	if (this->is_idle_) {
		return;
	}

	// Coordinates of the center point of the sprite
	float center_x = this->render_x_ + (UNIT_RENDER_WIDTH / 2);
	float center_y = this->render_y_ + (UNIT_RENDER_HEIGHT / 2);

	float future_center_x = center_x;
	float future_center_y = center_y;

	Orientation future_orientation = this->orientation_;

	// Get future position and orientation based on direction of movement
	if (this->is_moving_up_) {
		future_center_y -= this->velocity_y_;
		future_orientation = Orientation::UP;
	}
	else if (this->is_moving_down_) {
		future_center_y += this->velocity_y_;
		future_orientation = Orientation::DOWN;
	}
	else if (this->is_moving_left_) {
		future_center_x -= this->velocity_x_;
		future_orientation = Orientation::LEFT;
	}
	else if (this->is_moving_right_) {
		future_center_x += this->velocity_x_;
		future_orientation = Orientation::RIGHT;
	}


	float current_tile_center_x = (this->current_tile_x_ * TILE_RENDER_WIDTH) + (float)TILE_RENDER_WIDTH / 2.0f;
	float current_tile_center_y = (this->current_tile_y_ * TILE_RENDER_HEIGHT) + (float)TILE_RENDER_HEIGHT / 2.0f;

	// Don't allow unit to move beyond the center of the tile
	if ((this->is_moving_left_ && future_center_x <= current_tile_center_x) ||
		(this->is_moving_right_ && future_center_x >= current_tile_center_x) ||
		(this->is_moving_up_ && future_center_y <= current_tile_center_y) ||
		(this->is_moving_down_ && future_center_y >= current_tile_center_y)
	) {
		// Get the next tile in the direction of movement (to the left)
		Tile*  next_tile = this->GetNextTileInDirection(future_orientation);

		// If the next tile is out of bounds or solid => stop (in the center)
		if (next_tile == nullptr || (next_tile != nullptr && next_tile->is_solid_)) {
			this->StopMoving();
			return;
		}
	}


	// Get the tile of the future position
	Tile* future_tile = GetTileForCenterUnitCoordinates(future_center_x, future_center_y);

	// Move to the new position
	this->render_x_ = future_center_x - ((float)UNIT_RENDER_WIDTH / 2.0f);
	this->render_y_ = future_center_y - ((float)UNIT_RENDER_HEIGHT / 2.0f);

	// Update the current tile
	this->current_tile_x_ = future_tile->map_x_;
	this->current_tile_y_ = future_tile->map_y_;
	this->current_tile_ = future_tile;

	// Update orientation
	this->orientation_ = future_orientation;
}

void Unit::ManageAnimation(float delta_time) {
	this->animation_timer_ += delta_time;

	if (this->animation_timer_ >= this->animation_delay_)
	{
		this->current_animation_frame_++;

		if (this->current_animation_frame_ >= this->animation_frames_count_)
		{
			this->current_animation_frame_ = 0;
		}

		this->animation_timer_ = 0.0;
	}
}

void Unit::Update(float delta_time, const Uint8* keyboard_state) {
	this->ManageMovement(delta_time);
	if (!this->is_idle_) {
		this->ManageAnimation(delta_time);
	}
}

void Unit::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	this->spritesheet_texture_ = asset_loader->units_spritesheet_;

	SDL_Rect spritesheet_rect;
	spritesheet_rect.x = this->spritesheet_x_ + (this->current_animation_frame_ * UNIT_SPRITE_WIDTH);
	spritesheet_rect.y = this->spritesheet_y_;
	spritesheet_rect.w = UNIT_SPRITE_WIDTH;
	spritesheet_rect.h = UNIT_SPRITE_HEIGHT;

	SDL_FRect render_rect;
	render_rect.x = this->render_x_;
	render_rect.y = this->render_y_;
	render_rect.w = this->render_width_;
	render_rect.h = this->render_height_;

	if (RENDER_UNITS_DEBUG) {
		// Draw sprite box
		SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);
		SDL_RenderDrawRectF(renderer, &render_rect);
	}

	// Render the unit sprite according to the orientation
	switch (this->orientation_)
	{
		case Orientation::UP:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 90, nullptr, SDL_FLIP_NONE);
			break;
		case Orientation::DOWN:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 270, nullptr, SDL_FLIP_NONE);
			break;
		case Orientation::LEFT:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 0, nullptr, SDL_FLIP_NONE);
			break;
		case Orientation::RIGHT:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
			break;
	}
	
	if (RENDER_UNITS_DEBUG) {
		// Draw center point in red
		SDL_FRect center_point_rect;
		center_point_rect.x = this->render_x_ + (UNIT_RENDER_WIDTH / 2);
		center_point_rect.y = this->render_y_ + (UNIT_RENDER_HEIGHT / 2);
		center_point_rect.w = 2;
		center_point_rect.h = 2;

		SDL_SetRenderDrawColor(renderer, 200, 25, 25, 0);
		SDL_RenderDrawRectF(renderer, &center_point_rect);

		// Draw current tile in red
		SDL_FRect current_tile_rect;
		current_tile_rect.x = this->current_tile_x_ * TILE_RENDER_WIDTH;
		current_tile_rect.y = this->current_tile_y_ * TILE_RENDER_HEIGHT;
		current_tile_rect.w = TILE_RENDER_WIDTH;
		current_tile_rect.h = TILE_RENDER_HEIGHT;

		SDL_SetRenderDrawColor(renderer, 200, 25, 25, 0);
		SDL_RenderDrawRectF(renderer, &current_tile_rect);
	}
}
