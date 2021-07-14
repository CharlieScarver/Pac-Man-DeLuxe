#include "PacMan.h"

PacMan::PacMan(float x, float y, float width, float height, Map* map) : Unit(x, y, width, height, map) {

	this->lives_ = 1;

	this->spritesheet_x_ = PACMAN_SPRITESHEET_X;
	this->spritesheet_y_ = PACMAN_SPRITESHEET_Y;
	this->animation_delay_ = PACMAN_ANIMATION_DELAY;
	this->animation_frames_count_ = PACMAN_ANIMATION_FRAMES;

	this->velocity_x_ = PACMAN_DEFAULT_VELOCITY_X;
	this->velocity_y_ = 0;
}

PacMan::~PacMan() {}

void PacMan::HandleInput() {}

void PacMan::Update(float delta_time, Map* map) {
	Unit::Update(delta_time, map);

	this->HandleInput();
}

void PacMan::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);
}

