#pragma once
#include "GameObject.h"

// Forward declaration to resolve circular dependency
class Tile;
class Map;

#define UNIT_SPRITE_WIDTH 16
#define UNIT_SPRITE_HEIGHT 16
#define UNIT_RENDER_WIDTH 48
#define UNIT_RENDER_HEIGHT 48

#define UNIT_TURN_PIXEL_RADIUS 2

#define RENDER_UNITS_DEBUG 1

typedef enum class OrientationEnum {
	LEFT,
	RIGHT,
	UP,
	DOWN
} Orientation;

class Unit : public GameObject {
protected:
	// Spritesheet source rect
	int spritesheet_x_;
	int spritesheet_y_;

	SDL_Texture* spritesheet_texture_;

	// Animation
	int current_animation_frame_;
	int animation_frames_count_;
	float animation_timer_;
	int animation_delay_; // static? no need?

	// Movement
	bool is_idle_;
	bool is_moving_left_;
	bool is_moving_right_;
	bool is_moving_up_;
	bool is_moving_down_;
	bool is_cornering_;

	// Velocity
	float velocity_x_;
	float velocity_y_;

	Map* map_;

	Unit(float x, float y, float width, float height, Map* map);

	Tile* GetTileForUnitCoordinates(float x, float y);
	Tile* GetTileForCenterUnitCoordinates(float center_x, float center_y);
	void SetCenterToTileCenter(Tile* tile);
	void StopMoving();
	Tile* GetNextTileInDirection(Orientation direction);

	void ManageMovement(float delta_time);
	void ManageAnimation(float delta_time);

public:
	// Current tile
	int current_tile_x_;
	int current_tile_y_;
	Tile* current_tile_;

	Orientation orientation_;

	~Unit();

	virtual void Update(float delta_time, const Uint8* keyboard_state);
	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
