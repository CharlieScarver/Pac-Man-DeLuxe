#include "Game.h"
#include <iostream>

Game::Game() {
	this->is_running_ = false;
	this->window_ = nullptr;
	this->renderer_ = nullptr;

	this->texture_ = nullptr;
	this->source_rect_ = nullptr;
	this->destination_rect_ = nullptr;

	this->asset_loader_ = nullptr;
	this->map_ = nullptr;

	this->is_idle_ = false;
	this->is_moving_left_ = false;
	this->is_moving_right_ = false;
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


	this->window_ = SDL_CreateWindow(title, x, y, width, height, flags);
	if (this->window_ == nullptr) {
		std::cout << "Error creating window:" << SDL_GetError() << std::endl;
	}
	std::cout << "Window created." << std::endl;

	this->renderer_ = SDL_CreateRenderer(window_, -1, 0);
	if (this->renderer_ == nullptr) {
		std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
	}
	std::cout << "Renderer created." << std::endl;

	// ==============
	
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

	// ==============

	this->map_ = new Map();
	map_->LoadMapFromFile("Assets/Maps/level0.txt");

	// ==============

	this->destination_rect_ = new SDL_FRect();
	this->destination_rect_->x = 84;
	this->destination_rect_->y = 84;
	this->destination_rect_->w = 48;
	this->destination_rect_->h = 48;

	// Image to Surface to Texture
	SDL_Surface* tempSurface = IMG_Load("Assets/Spritesheets/pac-man-ghosts-sprites.png");
	this->texture_ = SDL_CreateTextureFromSurface(this->renderer_, tempSurface);
	// Free the surface
	SDL_FreeSurface(tempSurface);

	return 0;
}

void Game::Events(SDL_Event* event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			// Check which key was pressed
			switch (event->key.keysym.sym) {
				case SDLK_LEFT:
					this->is_moving_left_ = true;
					this->is_moving_right_ = false;
					this->is_idle_ = false;
					break;
				case SDLK_RIGHT:
					this->is_moving_left_ = false;
					this->is_moving_right_ = true;
					this->is_idle_ = false;
					break;
				case SDLK_UP:
				
					break;
				case SDLK_DOWN:
				
					break;
				case SDLK_ESCAPE:
					this->is_running_ = false;
					std::cout << "Quit event received. (Esc)" << std::endl;
					break;
			}
			break;
		case SDL_KEYUP:
			// Check which key was pressed
			switch (event->key.keysym.sym) {
				case SDLK_LEFT:
					this->is_moving_left_ = false;
					break;
				case SDLK_RIGHT:
					this->is_moving_right_ = false;
					break;
				}
			break;
		case SDL_QUIT:
			this->is_running_ = false;
			std::cout << "Quit event received. (Close)" << std::endl;
			break;
	}
}

void Game::Update(float delta_time) {
	if (this->is_moving_left_) {
		this->destination_rect_->x -= delta_time;
	} else if (this->is_moving_right_) {
		this->destination_rect_->x += delta_time;
	}
}

void Game::Render() {
	// Rendering functions updates a backbuffer, instead of  the screen directly
	if (SDL_SetRenderDrawColor(this->renderer_, 100, 149, 240, 0) != 0) {
		std::cout << "Set draw color error: " << SDL_GetError() << std::endl;
	}

	// Clear the current rendering target with the drawing color
	if (SDL_RenderClear(this->renderer_) != 0) {
		std::cout << "Render clear error: " << SDL_GetError() << std::endl;
	}

	int current_frame = 0;
	SDL_Rect source_rect;
	source_rect.x = current_frame * 16;
	source_rect.y = 0;
	source_rect.w = 16;
	source_rect.h = 16;

	if (SDL_RenderCopyExF(this->renderer_, this->texture_, &source_rect, this->destination_rect_, 0, NULL, SDL_FLIP_HORIZONTAL) != 0) {
		std::cout << "Render copy ex error: " << SDL_GetError() << std::endl;
	}

	this->map_->Render(this->renderer_, this->asset_loader_);

	// Update the screen with the composed backbuffer
	SDL_RenderPresent(this->renderer_);
}

void Game::Cleanup() {
	// Free allocated memory and cleanup libraries loaded in asset loader
	delete this->map_;
	delete this->asset_loader_;
	delete this->destination_rect_;

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

	while (this->is_running_) {
		while (SDL_PollEvent(&Event)) {
			this->Events(&Event);
		}

		// Calculate delta time
		const Uint64 now = SDL_GetPerformanceCounter();
		const float delta_time = (float)((now - before) * 1000 / (float)SDL_GetPerformanceFrequency());
		before = now;

		this->Update(delta_time);
		this->Render();
	}

	this->Cleanup();

	std::cout << "Closing..." << std::endl;
	return 0;
}
