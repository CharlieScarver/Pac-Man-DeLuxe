#include "PacMan.h"
#include "Map.h"

PacMan::PacMan(float x, float y, float width, float height, Map* map) : Unit(x, y, width, height, map) {

	this->lives_ = 1;

	this->spritesheet_position_ = Vector2(PACMAN_SPRITESHEET_X, PACMAN_SPRITESHEET_Y);

	this->animation_frames_count_ = PACMAN_ANIMATION_FRAMES;
	this->animation_delay_ = PACMAN_ANIMATION_DELAY;

	this->velocity_x_ = PACMAN_DEFAULT_VELOCITY;
	this->velocity_y_ = PACMAN_DEFAULT_VELOCITY;
}

PacMan::~PacMan() {}

void PacMan::HandleInput(const Uint8* keyboard_state) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Get the current tile center
	Vector2 current_tile_center = Utilities::GetCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

	// Get the rendered sprite center
	Vector2 render_center = Utilities::GetCenterPointOfRectangle(this->render_position_, this->render_size_);

	bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - UNIT_TURN_PIXEL_RADIUS && render_center.x_ <= current_tile_center.x_ + UNIT_TURN_PIXEL_RADIUS;
	bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - UNIT_TURN_PIXEL_RADIUS && render_center.y_ <= current_tile_center.y_ + UNIT_TURN_PIXEL_RADIUS;
	
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

void PacMan::Update(float delta_time, const Uint8* keyboard_state) {
	this->HandleInput(keyboard_state);

	Unit::Update(delta_time, keyboard_state);
}

void PacMan::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}

