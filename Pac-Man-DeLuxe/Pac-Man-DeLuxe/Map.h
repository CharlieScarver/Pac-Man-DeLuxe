#pragma once
#include "Tile.h"
#include "PacMan.h"

#define MAP_WIDTH_IN_TILES 28
#define MAP_HEIGHT_IN_TILES 36
#define MAP_OFFSET_X 0
#define MAP_OFFSET_Y 0

class Map final {
private:
	char* file_name_;
	Tile* tile_matrix_[MAP_WIDTH_IN_TILES][MAP_HEIGHT_IN_TILES];
public:
	Map();
	~Map();

	PacMan* pacman_;

	void LoadMapFromFile(const char* file_name);
	Tile* GetTile(int x, int y);
	void SetTile(int x, int y, Tile* tile);

	bool DetermineIfTileIsTurn(int x, int y);

	void Update(float delta_time, const Uint8* keyboard_state);
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader);
};
