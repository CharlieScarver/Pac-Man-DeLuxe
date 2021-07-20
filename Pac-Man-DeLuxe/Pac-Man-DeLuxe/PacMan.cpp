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
	// Get the current tile
	Tile* current_tile = this->map_->GetTile(this->current_tile_x_, this->current_tile_y_);
	float current_tile_center_x = current_tile->render_x_ + (float)TILE_RENDER_WIDTH / 2.0f;
	float current_tile_center_y = current_tile->render_y_ + (float)TILE_RENDER_HEIGHT / 2.0f;

	float center_x = this->render_x_ + (float)UNIT_RENDER_WIDTH / 2.0f;
	float center_y = this->render_y_ + (float)UNIT_RENDER_HEIGHT / 2.0f;

	bool center_x_in_turn_interval = center_x >= current_tile_center_x - UNIT_TURN_PIXEL_RADIUS && center_x <= current_tile_center_x + UNIT_TURN_PIXEL_RADIUS;
	bool center_y_in_turn_interval = center_y >= current_tile_center_y - UNIT_TURN_PIXEL_RADIUS && center_y <= current_tile_center_y + UNIT_TURN_PIXEL_RADIUS;
	
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
		// Change movement only if [idle], [moving in the opposite direction] or [on a turn tile and within turn interval]
		if (this->is_idle_ || this->is_moving_down_ || (current_tile->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->GetNextTileInDirection(Orientation::UP);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (!this->is_moving_up_) {
				this->SetCenterToTileCenter(current_tile);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = true;
			this->is_moving_down_ = false;
			this->is_moving_left_ = false;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
		if (this->is_idle_ || this->is_moving_up_ || (current_tile->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->GetNextTileInDirection(Orientation::DOWN);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}

			// If pacman is turning => center him on the current tile
			if (!this->is_moving_down_) {
				this->SetCenterToTileCenter(current_tile);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = false;
			this->is_moving_down_ = true;
			this->is_moving_left_ = false;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
		if (this->is_idle_ || this->is_moving_right_ || (current_tile->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->GetNextTileInDirection(Orientation::LEFT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (!this->is_moving_left_) {
				this->SetCenterToTileCenter(current_tile);
			}

			this->is_idle_ = false;
			this->is_moving_up_ = false;
			this->is_moving_down_ = false;
			this->is_moving_left_ = true;
			this->is_moving_right_ = false;
		}
	}
	else if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
		if (this->is_idle_ || this->is_moving_left_  || (current_tile->is_turn_tile_ && center_x_in_turn_interval && center_y_in_turn_interval)) {
			// Don't allow turning into a wall to stop yourself on an intersection
			Tile* next_tile = this->GetNextTileInDirection(Orientation::RIGHT);
			if (next_tile != nullptr && next_tile->is_solid_) {
				return;
			}
			
			// If pacman is turning => center him on the current tile
			if (!this->is_moving_right_) {
				this->SetCenterToTileCenter(current_tile);
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

