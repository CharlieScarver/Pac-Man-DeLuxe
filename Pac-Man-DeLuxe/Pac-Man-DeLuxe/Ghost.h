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

#define RENDER_GHOSTS_DEBUG 0

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
	static const int pinky_spritesheet_x_ = 32;
	static const int pinky_spritesheet_y_ = 16;
	static const int inky_spritesheet_x_ = 0;
	static const int inky_spritesheet_y_ = 32;
	static const int clyde_spritesheet_x_ = 32;
	static const int clyde_spritesheet_y_ = 32;

	static const int ghost_animation_frames_ = 2;
	static const int ghost_animation_delay_ = 200;

	// Movement
	static const float ghost_default_velocity_;

	// Turning
	bool is_turning_;

	/// <summary>
	/// The tile which the ghost is trying to reach.
	/// The target tuke can change dynamically.
	/// </summary>
	Tile* target_tile_;

	// Path finding
	std::vector<Tile*> reversed_path_to_target_;

	// Render path finding
	std::vector<std::vector<Tile*>> visited_layers_;

protected:

	/// <summary>
	/// Handles the behaviour and choices of the ghosts.
	/// </summary>
	void AI();

	/// <summary>
	/// Find the shortest path on the map from a given start tile to a given goal tile.
	/// Returns a vector of tiles representing the shortest path.
	/// The vector is reversed (from goal to start).
	/// Uses Breath-first search internally.
	/// </summary>
	std::vector<Tile*> FindShortestPath(Tile* start, Tile* goal);
	
	/// <summary>
	/// Returns the direction in which the ghost has to turn next.
	/// Usually the direction follows the shortest path to a target tile but not always.
	/// </summary>
	Direction GetNextTurnDirection();

public:
	/// <summary>
	/// Type of the ghost.
	/// The four possible types are: Blinky, Pinky, Inky and Clyde.
	/// </summary>
	GhostType ghost_type_;

	Ghost(float x, float y, Map* map, GhostType ghost_type);
	~Ghost();

	void Update(float delta_time, const Uint8* keyboard_state) override;
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
