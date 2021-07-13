#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "AssetLoader.h"
#include "Map.h"

class Game final {
private:
	// Original 224x288
	// Scale 3:1
	const int default_window_width = 672;
	const int default_window_height = 864;

	bool is_running_;
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	SDL_Texture* texture_;
	SDL_Rect* source_rect_;
	SDL_FRect* destination_rect_;

	bool is_moving_left_;
	bool is_moving_right_;
	bool is_idle_;

	int Init(const char* title, int x, int y, int width, int height, bool fullscreen);
	void Update(float delta_time);
	void Events(SDL_Event* event);
	void Render();
	void Cleanup();

public:
	AssetLoader* asset_loader_;
	Map* map_;

	Game();
	~Game();

	int Run();
};
