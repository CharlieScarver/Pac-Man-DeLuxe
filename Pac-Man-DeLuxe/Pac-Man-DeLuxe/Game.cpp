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
}

Game::~Game() {
}

int Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
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

	this->destination_rect_ = new SDL_Rect();
	this->destination_rect_->x = 0;
	this->destination_rect_->y = 0;
	this->destination_rect_->w = 48;
	this->destination_rect_->h = 48;

	// Image to Surface to Texture
	SDL_Surface* tempSurface = IMG_Load("Assets/Spritesheets/pac-man-ghosts-sprites.png");
	this->texture_ = SDL_CreateTextureFromSurface(this->renderer_, tempSurface);
	// Free the surface
	SDL_FreeSurface(tempSurface);

	return 0;
}

void Game::events(SDL_Event* event) {
	switch (event->type) {
	case SDL_QUIT:
		this->is_running_ = false;
		std::cout << "Quit event received." << std::endl;
		break;
	}
}

void Game::update() {

}

void Game::render() {
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

	this->destination_rect_->x = 84;
	this->destination_rect_->y = 84;

	if (SDL_RenderCopyEx(this->renderer_, this->texture_, &source_rect, this->destination_rect_, 0, NULL, SDL_FLIP_HORIZONTAL) != 0) {
		std::cout << "Render copy ex error: " << SDL_GetError() << std::endl;
	}

	this->map_->Render(this->renderer_, this->asset_loader_);

	// Update the screen with the composed backbuffer
	SDL_RenderPresent(this->renderer_);
}

void Game::cleanup() {
	// Free allocated memory and cleanup libraries loaded in asset loader
	delete this->map_;
	delete this->asset_loader_;
	delete this->destination_rect_;

	// Close and cleanup all SDL systems
	SDL_Quit();
	std::cout << "Cleanup finished." << std::endl;
}

int Game::run() {
	if (this->init("SDL Game Window", 50, 50, this->default_window_width, this->default_window_height, false) != 0) {
		this->cleanup();
		return -1;
	}

	SDL_Event Event;
	while (this->is_running_) {
		while (SDL_PollEvent(&Event)) {
			this->events(&Event);
		}

		this->update();
		this->render();
	}

	this->cleanup();

	std::cout << "Closing..." << std::endl;
	return 0;
}
