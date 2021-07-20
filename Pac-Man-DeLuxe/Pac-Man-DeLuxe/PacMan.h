#pragma once
#include "Unit.h"

#define PACMAN_SPRITESHEET_X 0
#define PACMAN_SPRITESHEET_Y 0
#define PACMAN_ANIMATION_FRAMES 2
#define PACMAN_ANIMATION_DELAY 100
// Original Pac-Man speed is 1.46 pixels per frame
#define PACMAN_DEFAULT_VELOCITY 1.46f * 1.12f

// Bottom row sprint time:
// ~3.80 s neworion
// ~3.43 s nes
// ~3.74 s mine

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
