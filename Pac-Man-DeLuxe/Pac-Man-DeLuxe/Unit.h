#pragma once
#include "GameObject.h"
#include "Orientation.h"
#include "Direction.h"
#include "Timer.h"

// Forward declaration to resolve circular dependency
class Tile;
class Map;

class Unit : public GameObject {
protected:
	// Sizes
	static const int sprite_width_ = 16;
	static const int sprite_height_ = 16;
	static const int render_width_ = 48;
	static const int render_height_ = 48;

	static const int input_delay_ = 150;

	// The loaded spritesheet texture
	SDL_Texture* spritesheet_texture_;

	// Animation
	
	/// <summary>
	/// The index of the current animation frame.
	/// </summary>
	int current_animation_frame_;
	
	/// <summary>
	/// The total count of animation frames for the current animation.
	/// </summary>
	int animation_frames_count_;

	/// <summary>
	/// The animation timer which counts how much time has passed since the last animation frame change.
	/// </summary>
	float animation_timer_; // in ms

	/// <summary>
	/// The time duration between animation frames.
	/// </summary>
	int animation_delay_; // in ms

	// Velocity

	/// <summary>
	/// The horizontal velocity of the unit in pixels per frame.
	/// </summary>
	float velocity_x_; // in pixels per frame

	/// <summary>
	/// The vertical velocity of the unit in pixels per frame.
	/// </summary>
	float velocity_y_; // in pixels per frame

	/// <summary>
	/// The game map.
	/// Used to access tiles and Pac-Man.
	/// </summary>
	Map* map_;

	/// <summary>
	/// Shows if debug info will be rendered or not.
	/// </summary>
	bool render_debug_;

	// Timers
	Timer input_timer_;

	Unit(float x, float y, float width, float height, Map* map);

	/// <summary>
	/// Returns the tile in which the center of the unit with given render coordinates is located.
	/// </summary>
	Tile* GetTileForUnitCoordinates(float x, float y);

	/// <summary>
	/// Returns the tile in which the given unit center coordinates are located.
	/// </summary>
	Tile* GetTileForCenterUnitCoordinates(float center_x, float center_y);

	/// <summary>
	/// Set the center of the unit to the center of the given tile.
	/// </summary>
	void SetCenterToTileCenter(Tile* tile);

	/// <summary>
	/// Stop the unit on its current location.
	/// </summary>
	void StopMoving();

	/// <summary>
	/// The basic movement logic of the unit.
	/// </summary>
	void ManageMovement(float delta_time);

	/// <summary>
	/// The basic animation logic of the unit.
	/// </summary>
	void ManageAnimation(float delta_time);

public:
	/// <summary>
	/// The tile in which the unit is currently located.
	/// </summary>
	Tile* current_tile_;

	/// <summary>
	/// The orientation of the unit.
	/// Possible values: Up, Down, Left, Right.
	/// </summary>
	Orientation orientation_;

	/// <summary>
	/// The movement direction of the unit.
	/// Possible values: None, Up, Down, Left, Right.
	/// </summary>
	Direction direction_;

	~Unit() = default;

	virtual void Update(float delta_time, const Uint8* keyboard_state);
	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
