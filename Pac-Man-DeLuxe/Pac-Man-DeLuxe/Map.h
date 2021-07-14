#pragma once
#include "Tile.h"
#include "PacMan.h"

#define MAP_WIDTH_IN_TILES 12
#define MAP_HEIGHT_IN_TILES 12
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

	void Update(float delta_time);
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader);
};
