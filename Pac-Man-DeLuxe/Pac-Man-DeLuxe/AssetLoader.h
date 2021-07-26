#pragma once
#include "SDL_image.h"

class AssetLoader final {
public:
	AssetLoader();
	/// Unload the loaded assets and SDL image libraries.
	~AssetLoader();

	/// <summary>
	/// The spritesheet used for the units.
	/// </summary>
	SDL_Texture* units_spritesheet_;

	/// <summary>
	/// The spritesheet used for the tiles.
	/// </summary>
	SDL_Texture* tiles_spritesheet_;

	/// <summary>
	/// The spritesheet usded for letters and labels.
	/// </summary>
	SDL_Texture* letters_spritesheet_;

	/// <summary>
	/// Initialize SDL Image.
	/// </summary>
	/// <returns></returns>
	int Init();
	
	/// <summary>
	/// Loads the predefined assets.
	/// </summary>
	int LoadAssets(SDL_Renderer* renderer);
};
