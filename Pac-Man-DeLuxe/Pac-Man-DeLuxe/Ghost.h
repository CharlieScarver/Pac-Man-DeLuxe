#pragma once
#include "Unit.h"
#include "GhostType.h"
#include "Tile.h"
#include "Utilities.h"

#define GHOST_SPRITESHEET_X 0
#define GHOST_SPRITESHEET_Y 0
#define GHOST_ANIMATION_FRAMES 2
#define GHOST_ANIMATION_DELAY 100
// Original ghost speed is 1.46 pixels per frame
#define GHOST_DEFAULT_VELOCITY 1.46f * 1.12f

class Ghost final : public Unit {
private:
	// Sizes
	static const int ghost_sprite_width_ = 16;
	static const int ghost_sprite_height_ = 16;
	static const int ghost_render_width_ = 48;
	static const int ghost_render_height_ = 48;

	// Animation
	static const int blinky_spritesheet_x_ = 0;
	static const int blinky_spritesheet_y_ = 16;
	static const int pinky_spritesheet_x_ = 16;
	static const int pinky_spritesheet_y_ = 16;
	static const int inky_spritesheet_x_ = 0;
	static const int inky_spritesheet_y_ = 32;
	static const int clyde_spritesheet_x_ = 16;
	static const int clyde_spritesheet_y_ = 32;

	static const int ghost_animation_frames_ = 2;
	static const int ghost_animation_delay_ = 200;

	// Movement
	static const float ghost_default_velocity_;

	// Turning
	bool is_turning_;
	Direction next_turn_;

	// Path finding
	std::vector<Tile*> reversed_path_to_target_;

protected:

	void AI();
	std::vector<Tile*> FindShortestPath(Tile* start, Tile* goal);

public:
	GhostType ghost_type_;

	Tile* target_tile_;

	Ghost(float x, float y, Map* map, GhostType ghost_type);
	~Ghost();

	void Update(float delta_time, const Uint8* keyboard_state) override;
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
