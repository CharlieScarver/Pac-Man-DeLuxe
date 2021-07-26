#pragma once
#include "GameObject.h"
#include "Orientation.h"
#include "Direction.h"

// Forward declaration to resolve circular dependency
class Tile;
class Map;

#define RENDER_UNITS_DEBUG 1

class Unit : public GameObject {
protected:
	// The loaded spritesheet texture
	SDL_Texture* spritesheet_texture_;

	// Animation
	int current_animation_frame_;
	int animation_frames_count_;
	float animation_timer_;
	int animation_delay_;

	// Velocity
	float velocity_x_;
	float velocity_y_;

	Map* map_;

	Unit(float x, float y, float width, float height, Map* map);

	Tile* GetTileForUnitCoordinates(float x, float y);
	Tile* GetTileForCenterUnitCoordinates(float center_x, float center_y);
	void SetCenterToTileCenter(Tile* tile);
	void StopMoving();

	void ManageMovement(float delta_time);
	void ManageAnimation(float delta_time);

protected:
	// Sizes
	static const int sprite_width_ = 16;
	static const int sprite_height_ = 16;
	static const int render_width_ = 48;
	static const int render_height_ = 48;

public:
	// Current tile
	Tile* current_tile_;

	Orientation orientation_;
	Direction direction_;

	~Unit() = default;

	virtual void Update(float delta_time, const Uint8* keyboard_state);
	virtual void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
