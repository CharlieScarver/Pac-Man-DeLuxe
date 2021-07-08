#pragma once
#include "Tile.h"
#include <vector>

class Map final {
private:
	const int offset_x_ = 0;
	const int offset_y_ = 0;
	const int TILE_WIDTH_ = 24;
	const int TILE_HEIGHT_ = 24;

	char* file_name_;
	std::vector<std::vector<Tile*>> tile_matrix_;
public:
	int size_x_;
	int size_y_;

	Map(int x, int y);
	~Map();

	void LoadMapFromFile(const char* file_name);
	Tile* GetTile(int x, int y);
	void SetTile(int x, int y, Tile* tile);

	void Render(SDL_Renderer* renderer);
};
