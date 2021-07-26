#pragma once
#include "Unit.h"
#include "Timer.h"

// Bottom row sprint time:
// ~3.80 s neworion
// ~3.43 s nes
// ~3.74 s mine

class PacMan final : public Unit {
private:
	static const int spritesheet_x_ = 0;
	static const int spritesheet_y_ = 0;

	static const int animation_frames_ = 2;
	static const int default_animation_delay_ = 100; // in ms
	static const int energized_animation_delay_ = 75;

	// Movement

	// Original Pac-Man speed is 1.46 pixels per frame
	static const float default_velocity_;
	// Energized velocity is 110% of original velocity
	static const float energized_velocity_;

	/// <summary>
	/// The radius of the interval in which Pac-Man is allow to turn.
	/// The center point of the interval is the center point of the corresponding turn tile.
	/// </summary>
	static const float turn_radius_; // in pixels

	static const int energized_duration_ = 5000; // in ms

	/// <summary>
	/// Number of update frames to be skipped
	/// </summary>
	int skip_frames = 0;

	/// <summary>
	/// Shows how much score does the player have.
	/// </summary>
	int score_;

	/// <summary>
	/// Shows if Pac-Man is energized or not.
	/// Pac-Man gets energized temporarily after eating an energizer.
	/// </summary>
	bool is_energized_;

	// Timers
	Timer energized_timer_;

	void HandleInput(const Uint8* keyboard_state);

	/// <summary>
	/// Energize Pac-Man after he eats an energizer.
	/// </summary>
	void Energize();

public:
	PacMan(float x, float y, Map* map);
	~PacMan() = default;

	/// <summary>
	/// Returns Pac-Man's score.
	/// </summary>
	int Score();

	/// <summary>
	/// Increases Pac-Man's score.
	/// </summary>
	void GainScore(int gained_score);

	void Update(float delta_time, const Uint8* keyboard_state) override;
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader) override;
};
