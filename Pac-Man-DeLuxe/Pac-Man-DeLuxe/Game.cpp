#include "Game.h"
#include <iostream>

Game::Game() {
	this->is_running_ = false;
	this->window_ = nullptr;
	this->renderer_ = nullptr;

	this->asset_loader_ = nullptr;
	this->map_ = nullptr;
}

Game::~Game() {
}

int Game::Init(const char* title, int x, int y, int width, int height, bool fullscreen) {
	this->is_running_ = true;

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// Returns -1 on an error or 0 on success.
	int sdl_init = SDL_Init(SDL_INIT_EVERYTHING);
	if (sdl_init != 0) {
		std::cout << "SDL initialization failed." << std::endl;
		return -1;
	}
	std::cout << "SDL initialized succesfully." << std::endl;

	// Create window
	this->window_ = SDL_CreateWindow(title, x, y, width, height, flags);
	if (this->window_ == nullptr) {
		std::cout << "Error creating window:" << SDL_GetError() << std::endl;
	}
	std::cout << "Window created." << std::endl;

	// Create renderer
	this->renderer_ = SDL_CreateRenderer(window_, -1, 0);
	if (this->renderer_ == nullptr) {
		std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
	}
	std::cout << "Renderer created." << std::endl;

	// Initialize asset loader and load the predefined assets
	this->asset_loader_ = new AssetLoader();
	if (this->asset_loader_->Init() != 0) {
		std::cout << "Error initializing asset loader: " << SDL_GetError() << std::endl;
		return -1;
	}

	if (this->asset_loader_->LoadAssets(renderer_) != 0) {
		std::cout << "Error loading assets: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Load map file
	this->map_ = new Map();
	map_->LoadMapFromFile("Assets/Maps/level1.txt");

	return 0;
}

void Game::Events(SDL_Event* event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			// Quit if Escape was pressed
			if (event->key.keysym.sym == SDLK_ESCAPE) {
				this->is_running_ = false;
				std::cout << "Quit event received. (Esc)" << std::endl;
			}
			break;
		case SDL_QUIT:
			this->is_running_ = false;
			std::cout << "Quit event received. (Close)" << std::endl;
			break;
	}
}

void Game::Update(float delta_time, const Uint8* keyboard_state) {
	// Update map
	this->map_->Update(delta_time, keyboard_state);
}

void Game::Render() {
	// Rendering functions updates a backbuffer, instead of  the screen directly
	// Cornflower blue  100, 149, 240
	if (SDL_SetRenderDrawColor(this->renderer_, 0, 0, 0, 0) != 0) {
		std::cout << "Set draw color error: " << SDL_GetError() << std::endl;
	}

	// Clear the current rendering target with the drawing color
	if (SDL_RenderClear(this->renderer_) != 0) {
		std::cout << "Render clear error: " << SDL_GetError() << std::endl;
	}

	// Render map
	this->map_->Render(this->renderer_, this->asset_loader_);

	// Update the screen with the composed backbuffer
	SDL_RenderPresent(this->renderer_);
}

void Game::Cleanup() {
	// Free allocated memory and cleanup libraries loaded in asset loader
	delete this->map_;
	delete this->asset_loader_;

	// Close and cleanup all SDL systems
	SDL_Quit();
	std::cout << "Cleanup finished." << std::endl;
}

int Game::Run() {
	if (this->Init("SDL Game Window", 50, 50, this->default_window_width, this->default_window_height, false) != 0) {
		this->Cleanup();
		return -1;
	}

	SDL_Event Event;
	Uint64 before = SDL_GetPerformanceCounter();

	// Frame time that would result in 60 fps
	const float desired_frame_time = 16.67f;

	float fps = 0;
	int fps_counter = 0;
	float fps_timer = 0;

	while (this->is_running_) {
		while (SDL_PollEvent(&Event)) {
			this->Events(&Event);
		}

		const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

		// Calculate delta time
		const Uint64 now = SDL_GetPerformanceCounter();
		const float delta_time = (float)(now - before) * 1000.0f / (float)SDL_GetPerformanceFrequency();
		before = now;

		// Cap FPS at 60
		float frame_time_diff = desired_frame_time - delta_time;
		if (frame_time_diff > 0) {
			// Wait out the difference in frame time
			SDL_Delay((Uint32)frame_time_diff);
		}

		// Calculate fps
		fps_counter++;
		fps_timer += delta_time + frame_time_diff;
		// Set the fps value every 1s = 1000ms
		if (fps_timer >= 1000) {
			fps = fps_counter;
			fps_counter = 0;
			fps_timer = 0;

			std::cout << "FPS: " << fps << std::endl;
		}

		this->Update(delta_time, keyboard_state);
		this->Render();
	}

	this->Cleanup();

	std::cout << "Closing..." << std::endl;
	return 0;
}
