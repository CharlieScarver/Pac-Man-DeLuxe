#include <iostream>
#include "Unit.h"
#include "Map.h"

Unit::Unit(float x, float y, float width, float height, Map* map) : GameObject(x, y, width, height) {
	// todo
	this->spritesheet_x_ = 0;
	this->spritesheet_y_ = 0;

	this->is_idle_ = true;
	this->is_moving_left_ = false;
	this->is_moving_right_ = false;
	this->is_moving_up_ = false;
	this->is_moving_down_ = false;

	// todo
	this->current_tile_x_ = 0;
	this->current_tile_y_ = 0;

	// todo
	this->current_animation_frame_ = 0;
	this->animation_frames_count_ = 1;
	this->animation_delay_ = 0;

	// todo
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

void Unit::ManageMovement(float delta_time) {

	// Coordinates of the center point of the sprite
	float center_x = this->render_x_ + (UNIT_RENDER_WIDTH / 2);
	float center_y = this->render_y_ + (UNIT_RENDER_HEIGHT / 2);

	float future_center_x = center_x;
	float future_center_y = center_y;

	Orientation new_orientation = this->orientation_;

	if (this->is_moving_left_) {
		future_center_x -= this->velocity_x_ * delta_time;
		new_orientation = Orientation::LEFT;
	} else if (this->is_moving_right_) {
		future_center_x += this->velocity_x_ * delta_time;
		new_orientation = Orientation::RIGHT;
	}

	if (this->is_moving_up_) {
		future_center_y -= this->velocity_y_ * delta_time;
		new_orientation = Orientation::UP;
	}
	else if (this->is_moving_down_) {
		future_center_y += this->velocity_y_ * delta_time;
		new_orientation = Orientation::DOWN;
	}

	// If not moving there's no need to proceed
	if (future_center_x == this->render_x_ && future_center_y == this->render_y_) {
		return;
	}






	float current_tile_center_x = (this->current_tile_x_ * TILE_RENDER_WIDTH) + (float)TILE_RENDER_WIDTH / 2.0f;
	float current_tile_center_y = (this->current_tile_y_ * TILE_RENDER_HEIGHT) + (float)TILE_RENDER_HEIGHT / 2.0f;

	Tile* next_tile = nullptr;
	if (this->is_moving_left_ && future_center_x <= current_tile_center_x) {
	
		// If unit is on the map border => stop
		if (this->current_tile_x_ == 0) {
			this->StopMoving();
			return;
		}

		// Get the next tile in the direction of movement (to the left)
		next_tile = this->map_->GetTile(this->current_tile_x_ - 1, this->current_tile_y_);

	} else if (this->is_moving_right_ && future_center_x >= current_tile_center_x) {

		// If unit is on the map border => stop
		if (this->current_tile_x_ == MAP_WIDTH_IN_TILES - 1) {
			this->StopMoving();
			return;
		}

		// Get the next tile in the direction of movement (to the right)
		next_tile = this->map_->GetTile(this->current_tile_x_ + 1, this->current_tile_y_);

	} else if (this->is_moving_up_ && future_center_y <= current_tile_center_y) {

		// If unit is on the map border => stop
		if (this->current_tile_y_ == 0) {
			this->StopMoving();
			return;
		}

		// Get the next tile in the direction of movement (to the right)
		next_tile = this->map_->GetTile(this->current_tile_x_, this->current_tile_y_ - 1);

	} else if (this->is_moving_down_ && future_center_y >= current_tile_center_y) {

		// If unit is on the map border => stop
		if (this->current_tile_y_ == MAP_HEIGHT_IN_TILES - 1) {
			this->StopMoving();
			return;
		}

		// Get the next tile in the direction of movement (to the right)
		next_tile = this->map_->GetTile(this->current_tile_x_, this->current_tile_y_ + 1);
	}

	// If the next tile is solid => stop
	if (next_tile != nullptr && next_tile->is_solid_) {
		this->StopMoving();
		return;
	}





	Tile* future_tile = GetTileForCenterUnitCoordinates(future_center_x, future_center_y);

	// If tile is solid stop the unit
	if (future_tile == nullptr || future_tile->is_solid_) {
		this->StopMoving();
		return;
	}

	std::cout << "Moving to (" << future_center_x << ", " << future_center_y << "), tile[ " << future_tile->map_x_ << " ][ " << future_tile->map_y_ << " ]" << std::endl;
	
	// Move to the new position
	this->render_x_ = future_center_x - ((float)UNIT_RENDER_WIDTH / 2.0f);
	this->render_y_ = future_center_y- ((float)UNIT_RENDER_HEIGHT / 2.0f);

	// Update the current tile
	this->current_tile_x_ = future_tile->map_x_;
	this->current_tile_y_ = future_tile->map_y_;

	// Update orientation
	this->orientation_ = new_orientation;
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
	this->ManageAnimation(delta_time);
}

void Unit::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// Rendering functions updates a backbuffer, instead of the screen directly
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
		SDL_RenderDrawRectF(renderer, &render_rect);
	}

	// Render the unit sprite according to the orientation
	switch (this->orientation_)
	{
		case Orientation::LEFT:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 0, nullptr, SDL_FLIP_NONE);
			break;
		case Orientation::RIGHT:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
			break;
		case Orientation::UP:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 90, nullptr, SDL_FLIP_NONE);
			break;
		case Orientation::DOWN:
			SDL_RenderCopyExF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect, 270, nullptr, SDL_FLIP_NONE);
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
