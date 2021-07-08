#pragma once
#include "SDL.h"
#include "Map.h"

class Game {
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
	SDL_Rect* destination_rect_;

	int init(const char* title, int x, int y, int width, int height, bool fullscreen);
	void update();
	void events(SDL_Event* event);
	void render();
	void cleanup();

public:
	Map* map_;

	Game();
	~Game();

	int run();
};
