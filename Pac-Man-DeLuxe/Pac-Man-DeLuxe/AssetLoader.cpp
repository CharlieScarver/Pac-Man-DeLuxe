#include <iostream>
#include "AssetLoader.h"

AssetLoader::AssetLoader() {
	this->units_spritesheet_ = nullptr;
	this->tiles_spritesheet_ = nullptr;
}

AssetLoader::~AssetLoader() {
	this->units_spritesheet_ = nullptr;
	this->tiles_spritesheet_ = nullptr;

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
	SDL_Surface* tempSurface = IMG_Load("Assets/Spritesheets/pac-man-ghosts-sprites.png");
	if (tempSurface == nullptr) {
		std::cout << "IMG load error: " << SDL_GetError() << std::endl;
		return -1;
	}
	this->units_spritesheet_ = SDL_CreateTextureFromSurface(renderer, tempSurface);

	// Load the units spritesheet
	// Reuse tempSurface
	tempSurface = IMG_Load("Assets/Spritesheets/pac-man-tiles-sprites.png");
	if (tempSurface == nullptr) {
		std::cout << "IMG load error: " << SDL_GetError() << std::endl;
		return -1;
	}
	this->tiles_spritesheet_ = SDL_CreateTextureFromSurface(renderer, tempSurface);

	// Free the surface
	SDL_FreeSurface(tempSurface);

	std::cout << "Assets loaded." << std::endl;
	return 0;
}
