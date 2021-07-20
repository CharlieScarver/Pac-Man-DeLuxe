#include "PacMan.h"
#include "Map.h"

PacMan::PacMan(float x, float y, float width, float height, Map* map) : Unit(x, y, width, height, map) {

	this->lives_ = 1;

	this->spritesheet_x_ = PACMAN_SPRITESHEET_X;
	this->spritesheet_y_ = PACMAN_SPRITESHEET_Y;
	this->animation_frames_count_ = PACMAN_ANIMATION_FRAMES;
	this->animation_delay_ = PACMAN_ANIMATION_DELAY;

	this->velocity_x_ = PACMAN_DEFAULT_VELOCITY;
	this->velocity_y_ = PACMAN_DEFAULT_VELOCITY;
}

PacMan::~PacMan() {}

void PacMan::HandleInput(const Uint8* keyboard_state) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Get the current tile center
	Vector2 current_tile_center = Vector2::FindCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

	// Get the rendered sprite center
	Vector2 render_center = Vector2::FindCenterPointOfRectangle(this->render_position_, this->render_size_);

	bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - UNIT_TURN_PIXEL_RADIUS && render_center.x_ <= current_tile_center.x_ + UNIT_TURN_PIXEL_RADIUS;
	bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - UNIT_TURN_PIXEL_RADIUS && render_center.y_ <= current_tile_center.y_ + UNIT_TURN_PIXEL_RADIUS;
	
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
		// Change movement only if [idle], [moving in the opposite direction] or [on a turn tile and within turn interval]
		if (this->is_idle_ || this->is_moving_down_ || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Orientation::UP);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (!this->is_moving_up_) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = true;
			this->is_moving_down_ = false;
			this->is_moving_left_ = false;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
		if (this->is_idle_ || this->is_moving_up_ || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Orientation::DOWN);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (!this->is_moving_down_) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = false;
			this->is_moving_down_ = true;
			this->is_moving_left_ = false;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
		if (this->is_idle_ || this->is_moving_right_ || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Orientation::LEFT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (!this->is_moving_left_) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = false;
			this->is_moving_down_ = false;
			this->is_moving_left_ = true;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
		if (this->is_idle_ || this->is_moving_left_  || (this->current_tile_->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, Orientation::RIGHT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (!this->is_moving_right_) {
				this->SetCenterToTileCenter(this->current_tile_);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = false;
			this->is_moving_down_ = false;
			this->is_moving_left_ = false;
			this->is_moving_right_ = true;
		}
	}
}

void PacMan::Update(float delta_time, const Uint8* keyboard_state) {
	this->HandleInput(keyboard_state);

	Unit::Update(delta_time, keyboard_state);
}

void PacMan::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}

