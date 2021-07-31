#include "PacMan.h"
#include "Map.h"

// Original Pac-Man speed is 1.46 pixels per frame
const float PacMan::default_velocity_ = 1.46f * 1.12f;
// Energized velocity is 110% of original velocity
const float PacMan::energized_velocity_ = PacMan::default_velocity_ * 1.1f;

const float PacMan::turn_radius_ = 3.5f;

PacMan::PacMan(float x, float y, Map* map)
	: Unit(x, y, Unit::render_width_, Unit::render_height_, map), energized_timer_() {

	this->spritesheet_position_ = Vector2(PacMan::spritesheet_x_, PacMan::spritesheet_y_);

	this->animation_frames_count_ = PacMan::animation_frames_;
	this->animation_delay_ = PacMan::animation_delay_;

	this->velocity_x_ = PacMan::default_velocity_;
	this->velocity_y_ = PacMan::default_velocity_;

	this->score_ = 0;
	this->is_energized_ = false;
}

void PacMan::HandleInput(const Uint8* keyboard_state) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Get the current tile center
	Vector2F current_tile_center = Utilities::GetCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

	// Get the rendered sprite center
	Vector2F render_center = Utilities::GetCenterPointOfRectangle(this->render_position_, this->render_size_);

	bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - PacMan::turn_radius_ && render_center.x_ <= current_tile_center.x_ + PacMan::turn_radius_;
	bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - PacMan::turn_radius_ && render_center.y_ <= current_tile_center.y_ + PacMan::turn_radius_;
	
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
		// Change movement only if [idle], [moving in the opposite direction] or [on a turn tile and within turn interval]
		if (this->direction_ == Direction::NONE || this->direction_ == Direction::DOWN || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Direction::UP);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (this->direction_ != Direction::UP) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->direction_ = Direction::UP;
		}
	}
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
		if (this->direction_ == Direction::NONE || this->direction_ == Direction::UP || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Direction::DOWN);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (this->direction_ != Direction::DOWN) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->direction_ = Direction::DOWN;
		}
	}
	else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
		if (this->direction_ == Direction::NONE || this->direction_ == Direction::RIGHT || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Direction::LEFT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (this->direction_ != Direction::LEFT) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->direction_ = Direction::LEFT;
		}
	}
	else if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
		if (this->direction_ == Direction::NONE || this->direction_ == Direction::LEFT || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Direction::RIGHT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (this->direction_ != Direction::RIGHT) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->direction_ = Direction::RIGHT;
		}
	}
}

int PacMan::Score() {
	return this->score_;
}

void PacMan::GainScore(int gained_score) {
	this->score_ += gained_score;
}

void PacMan::Energize() {
	this->is_energized_ = true;

	// Start the energized timer
	this->energized_timer_.Start(PacMan::energized_duration_);
	
	// Update the animation delay and velocity
	this->animation_delay_ = PacMan::energized_animation_delay_;
	this->velocity_x_ = PacMan::energized_velocity_;
	this->velocity_y_ = PacMan::energized_velocity_;

	// Frighten the ghosts
	this->map_->FrightenGhosts();
}

void PacMan::Update(float delta_time, const Uint8* keyboard_state) {
	if (this->skip_frames == 0) {
		this->HandleInput(keyboard_state);

		Unit::Update(delta_time, keyboard_state);
	}
	else {
		this->skip_frames--;
	}

	// If the current tile contains an item
	if (this->current_tile_->contained_item_ != nullptr) {
		// Gain score
		this->score_ += this->current_tile_->contained_item_->score_;

		if (this->current_tile_->contained_item_->type_ == ItemType::PELLET) {
			// Skip one frame after consuming a pellet
			this->skip_frames = 1;
		}
		else if (this->current_tile_->contained_item_->type_ == ItemType::ENERGIZER) {
			// Skip three frames after consuming an energizer
			this->skip_frames = 3;
			this->Energize();
		}

		// Remove the item from the tile (memory will be freed by the Map later)
		this->current_tile_->contained_item_ = nullptr;
	}

	// If energized => update the energized timer
	if (this->is_energized_) {
		this->energized_timer_.Update(delta_time);
	}

	// If energized timer has completed => go back to normal state (deenergized)
	if (this->energized_timer_.HasCompleted()) {
		this->is_energized_ = false;
		this->animation_delay_ = PacMan::default_animation_delay_;
		this->velocity_x_ = PacMan::default_velocity_;
		this->velocity_y_ = PacMan::default_velocity_;
	}
}

void PacMan::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}

