#pragma once
#include "Unit.h"
#include "GhostType.h"
#include "Tile.h"
#include "Utilities.h"
#include "Timer.h"
#include "GhostMode.h"

#define RENDER_GHOSTS_DEBUG 1

class Ghost final : public Unit {
private:
	// Animation
	static const int blinky_spritesheet_x_ = 0;
	static const int blinky_spritesheet_y_ = 16;
	static const int pinky_spritesheet_x_ = 32;
	static const int pinky_spritesheet_y_ = 16;
	static const int inky_spritesheet_x_ = 0;
	static const int inky_spritesheet_y_ = 32;
	static const int clyde_spritesheet_x_ = 32;
	static const int clyde_spritesheet_y_ = 32;

	static const int frightened_spritesheet_x_ = 0;
	static const int frightened_spritesheet_y_ = 48;
	static const int eaten_spritesheet_x_ = -1;
	static const int eaten_spritesheet_y_ = -1;

	static const int animation_frames_ = 2;
	static const int animation_delay_ = 200;

	// Movement

	/// <summary>
	/// The speed of the ghost in pixels per frame.
	/// Original ghost speed is 1.46 pixels per frame (at 60 fps).
	/// </summary>
	static const float default_velocity_; // in pixels per frame
	
	/// <summary>
	///	Frightened velocity is 50% of normal velocity.
	/// Value is in pixels per frame.
	/// </summary>
	static const float frightened_velocity_;

	/// <summary>
	/// The radius of the interval in which Pac-Man is allow to turn.
	/// The center point of the interval is the center point of the corresponding turn tile.
	/// </summary>
	static const float turn_radius_; // in pixels

	static const int scatter_duration_ = 7000; // in ms
	static const int chase_duration_ = 20000;
	static const int frightened_duration_ = 5000;

	// Turning
	bool is_turning_;

	/// <summary>
	/// Type of the ghost.
	/// The possible types are: Blinky, Pinky, Inky and Clyde.
	/// </summary>
	GhostType type_;

	/// <summary>
	/// The tile which the ghost is trying to reach.
	/// The target tuke can change dynamically.
	/// </summary>
	Tile* target_tile_;

	// Path finding
	std::vector<Tile*> reversed_path_to_target_;

	// Render path finding
	std::vector<std::vector<Tile*>> visited_layers_;

	// Mode
	GhostMode previous_mode_;

	/// <summary>
	/// Mode of the ghost.
	/// The possible modes are: Scatter, Chase and Fright.
	/// </summary>
	GhostMode mode_;

	/// <summary>
	/// Shows if the ghost was eaten by Pac-Man.
	/// Once eaten the ghost will head towards the respawn tile to revive itself.
	/// </summary>
	bool is_eaten_;

	// Timers
	Timer scatter_timer_;
	Timer chase_timer_;
	Timer frightened_timer_;

	/// <summary>
	/// Handles the behaviour and choices of the ghosts.
	/// </summary>
	void AI(float delta_time);

	/// <summary>
	/// Returns the appropriate target tile based on mode and ghost type.
	/// </summary>
	/// <returns></returns>
	Tile* GetTargetTile();

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

	/// <summary>
	/// Returns a random valid direction to turn towards.
	/// </summary>
	/// <returns></returns>
	Direction GetRandomValidDirection();

	/// <summary>
	/// Change the ghost's mode to a given one.
	/// </summary>
	void ChangeMode(GhostMode new_mode);

public:

	Ghost(float x, float y, Map* map, GhostType type);
	~Ghost() = default;

	// Properties

	/// <summary>
	/// Mode of the ghost.
	/// The possible modes are: Scatter, Chase and Fright.
	/// </summary>
	GhostMode Mode();

	/// <summary>
	/// Shows if the ghost was eaten by Pac-Man.
	/// Once eaten the ghost will head towards the respawn tile to revive itself.
	/// </summary>
	bool IsEaten();

	// Methods

	/// <summary>
	/// Mark the ghost as eaten by Pac-Man
	/// </summary>
	void GetEaten();

	/// <summary>
	/// Frighten the ghost.
	/// </summary>
	void Frighten();

	void Update(float delta_time, const Uint8* keyboard_state) override;
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
