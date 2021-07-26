#include <iostream>
#include "AssetLoader.h"

AssetLoader::AssetLoader() {
	this->units_spritesheet_ = nullptr;
	this->tiles_spritesheet_ = nullptr;
	this->letters_spritesheet_ = nullptr;
}

AssetLoader::~AssetLoader() {
	// Can't delete the texture? (delete scalar error)

	// Unload the dynamically loaded image libraries
	IMG_Quit();
}

int AssetLoader::Init() {
	int format_flags = IMG_INIT_PNG;
	// Load the image libraries for the given formats
	int img_init = IMG_Init(format_flags);
	if ((img_init & format_flags) != format_flags) {
		std::cout << "IMG init error: " << SDL_GetError() << std::endl;
		return -1;
	}

	std::cout << "Asset loader initialized." << std::endl;
	return 0;
}

int AssetLoader::LoadAssets(SDL_Renderer* renderer) {
	// Load the units spritesheet
	// From Image to Surface to Texture
	SDL_Surface* units_surface = IMG_Load("Assets/Spritesheets/pac-man-ghosts-sprites.png");
	if (units_surface == nullptr) {
		std::cout << "IMG load error: " << SDL_GetError() << std::endl;
		return -1;
	}
	// Get the texture
	this->units_spritesheet_ = SDL_CreateTextureFromSurface(renderer, units_surface);
	// Free the surface
	SDL_FreeSurface(units_surface);
	units_surface = nullptr;

	// Load the units spritesheet
	// Reuse tempSurface
	SDL_Surface* tiles_surface = IMG_Load("Assets/Spritesheets/pac-man-tiles-sprites.png");
	if (tiles_surface == nullptr) {
		std::cout << "IMG load error: " << SDL_GetError() << std::endl;
		return -1;
	}
	// Get the texture
	this->tiles_spritesheet_ = SDL_CreateTextureFromSurface(renderer, tiles_surface);
	// Free the surface
	SDL_FreeSurface(tiles_surface);
	tiles_surface = nullptr;

	// Load the letters spritesheet
	// Reuse tempSurface
	SDL_Surface* letters_surface = IMG_Load("Assets/Spritesheets/pac-man-letters-sprites.png");
	if (letters_surface == nullptr) {
		std::cout << "IMG load error: " << SDL_GetError() << std::endl;
		return -1;
	}
	// Get the texture
	this->letters_spritesheet_ = SDL_CreateTextureFromSurface(renderer, letters_surface);
	// Free the surface
	SDL_FreeSurface(letters_surface);
	letters_surface = nullptr;

	std::cout << "Assets loaded." << std::endl;
	return 0;
}
