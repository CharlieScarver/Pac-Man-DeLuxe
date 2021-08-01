#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "AssetLoader.h"
#include "Map.h"

class Game final {
private:
	// Original 224x288
	// Scale 3:1
	static const int default_window_width_ = 672;
	static const int default_window_height_ = 864;

	static const int input_delay_ = 200;

	/// <summary>
	/// Shows if the game is running (if the main loop is being executed or not).
	/// </summary>
	bool is_running_;

	/// <summary>
	/// Shows if the game is paused or not (if the map is being updated).
	/// </summary>
	bool is_paused_;

	/// <summary>
	/// The SDL game window.
	/// </summary>
	SDL_Window* window_;

	/// <summary>
	/// The SDL renderer.
	/// </summary>
	SDL_Renderer* renderer_;

	/// <summary>
	/// The asset loader.
	/// Used to load, get and unload assets.
	/// </summary>
	AssetLoader* asset_loader_;

	/// <summary>
	/// The game map.
	/// Holds the tiles, items, units.
	/// </summary>
	Map* map_;

	// Timers
	Timer input_timer_;

	int Init(const char* title, int x, int y, int width, int height, bool fullscreen);
	void Update(float delta_time, const Uint8* keyboard_state);
	void Events(SDL_Event* event);
	void Render();
	void Cleanup();

public:
	Game();
	~Game();

	/// <summary>
	/// Shows if the game will stop for a restart.
	/// </summary>
	bool stop_and_restart_;

	/// <summary>
	/// Starts the game and keeps running until the game ends.
	/// </summary>
	int Run();
};
