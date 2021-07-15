#include "PacMan.h"

PacMan::PacMan(float x, float y, float width, float height, Map* map) : Unit(x, y, width, height, map) {

	this->lives_ = 1;

	this->spritesheet_x_ = PACMAN_SPRITESHEET_X;
	this->spritesheet_y_ = PACMAN_SPRITESHEET_Y;
	this->animation_delay_ = PACMAN_ANIMATION_DELAY;
	this->animation_frames_count_ = PACMAN_ANIMATION_FRAMES;

	this->velocity_x_ = PACMAN_DEFAULT_VELOCITY_X;
	this->velocity_y_ = PACMAN_DEFAULT_VELOCITY_Y;
}

PacMan::~PacMan() {}

void PacMan::HandleInput(const Uint8* keyboard_state) {
	const const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		this->is_idle_ = false;
		this->is_moving_up_ = true;
		this->is_moving_down_ = false;
		this->is_moving_left_ = false;
		this->is_moving_right_ = false;
	}
	else if (state[SDL_SCANCODE_S]) {
		this->is_idle_ = false;
		this->is_moving_up_ = false;
		this->is_moving_down_ = true;
		this->is_moving_left_ = false;
		this->is_moving_right_ = false;
	}
	else if (state[SDL_SCANCODE_A]) {
		this->is_idle_ = false;
		this->is_moving_up_ = false;
		this->is_moving_down_ = false;
		this->is_moving_left_ = true;
		this->is_moving_right_ = false;
	}
	else if (state[SDL_SCANCODE_D]) {
		this->is_idle_ = false;
		this->is_moving_up_ = false;
		this->is_moving_down_ = false;
		this->is_moving_left_ = false;
		this->is_moving_right_ = true;
	}
}

void PacMan::Update(float delta_time, const Uint8* keyboard_state) {
	this->HandleInput(keyboard_state);

	Unit::Update(delta_time, keyboard_state);
}

void PacMan::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}

