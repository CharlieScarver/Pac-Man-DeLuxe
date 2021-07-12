#pragma once
#include "SDL_image.h"

class AssetLoader {
public:
	AssetLoader();
	~AssetLoader();

	SDL_Texture* units_spritesheet_;
	SDL_Texture* tiles_spritesheet_;

	// SDL Image Initilization
	int Init();
	// Loads the predefined assets
	int LoadAssets(SDL_Renderer* renderer);
};
