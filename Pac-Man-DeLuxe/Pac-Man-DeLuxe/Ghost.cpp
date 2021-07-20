#include "Ghost.h"
#include "Map.h"

const float Ghost::ghost_default_velocity_ = 1.46f * 1.12f;

Ghost::Ghost(float x, float y, Map* map, GhostType ghost_type) : Unit(x, y, Ghost::ghost_render_width_, Ghost::ghost_render_height_, map) {

	this->ghost_type_ = ghost_type;

	switch (this->ghost_type_)
	{
	case GhostType::BLINKY:
		this->spritesheet_x_ = Ghost::blinky_spritesheet_x_;
		this->spritesheet_y_ = Ghost::blinky_spritesheet_y_;

		// Initial movement
		this->is_idle_ = false;
		this->is_moving_left_ = true;
		break;
	case GhostType::PINKY:
		this->spritesheet_x_ = Ghost::pinky_spritesheet_x_;
		this->spritesheet_y_ = Ghost::pinky_spritesheet_y_;
		break;
	case GhostType::INKY:
		this->spritesheet_x_ = Ghost::inky_spritesheet_x_;
		this->spritesheet_y_ = Ghost::inky_spritesheet_y_;

		// Initial movement
		this->is_idle_ = false;
		this->is_moving_right_ = true;
		break;
	case GhostType::CLYDE:
		this->spritesheet_x_ = Ghost::clyde_spritesheet_x_;
		this->spritesheet_y_ = Ghost::clyde_spritesheet_y_;
		break;
	}

	this->animation_frames_count_ = Ghost::ghost_animation_frames_;
	this->animation_delay_ = Ghost::ghost_animation_delay_;
	
	this->velocity_x_ = Ghost::ghost_default_velocity_;
	this->velocity_y_ = Ghost::ghost_default_velocity_;

}

Ghost::~Ghost() {}

void Ghost::AI() {
	// Get next tile in the facing direction
	Tile* next_tile = this->GetNextTileInDirection(this->orientation_);

	// If the next tile is solid turn around
	if (next_tile->is_solid_) {
		if (this->is_moving_left_) {
			this->is_idle_ = false;
			this->is_moving_left_ = false;
			this->is_moving_right_ = true;
		}
		else {
			this->is_idle_ = false;
			this->is_moving_left_ = true;
			this->is_moving_right_ = false;
		}
	}
}

void Ghost::Update(float delta_time, const Uint8* keyboard_state) {
	this->AI();

	Unit::Update(delta_time, keyboard_state);
}

void Ghost::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}
