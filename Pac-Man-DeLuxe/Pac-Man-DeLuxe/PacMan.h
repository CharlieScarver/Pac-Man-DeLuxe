#pragma once
#include "Unit.h"

#define PACMAN_SPRITESHEET_X 0
#define PACMAN_SPRITESHEET_Y 0
#define PACMAN_ANIMATION_FRAMES 2
#define PACMAN_ANIMATION_DELAY 500
#define PACMAN_DEFAULT_VELOCITY_X 0.15f
#define PACMAN_DEFAULT_VELOCITY_Y 0.15f

class PacMan final : public Unit {
protected:

	void HandleInput(const Uint8* keyboard_state);

public:
	int lives_;

	PacMan(float x, float y, float width, float height, Map* map);
	~PacMan();

	void Update(float delta_time, const Uint8* keyboard_state) override;
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
