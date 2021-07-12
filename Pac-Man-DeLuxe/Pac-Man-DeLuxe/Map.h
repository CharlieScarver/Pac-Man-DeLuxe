#pragma once
#include "Tile.h"

#define MAP_WIDTH_IN_TILES 12
#define MAP_HEIGHT_IN_TILES 12

class Map final {
private:
	const int offset_x_ = 0;
	const int offset_y_ = 0;

	char* file_name_;
	Tile* tile_matrix_[MAP_WIDTH_IN_TILES][MAP_HEIGHT_IN_TILES];
public:
	Map();
	~Map();

	void LoadMapFromFile(const char* file_name);
	Tile* GetTile(int x, int y);
	void SetTile(int x, int y, Tile* tile);

	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader);
};
