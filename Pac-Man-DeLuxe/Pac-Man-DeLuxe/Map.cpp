#include "Map.h"
#include <iostream>
#include <fstream>

Map::Map(int x, int y) {
	this->tile_matrix_ = std::vector<std::vector<Tile*>>(x, std::vector<Tile*>(y, nullptr));
	this->size_x_ = x;
	this->size_y_ = y;
}

Map::~Map() {}

void Map::LoadMapFromFile(const char* file_name) {
	std::ifstream ifs;

	// TODO: Prefix file name with Assets/
	ifs.open(file_name, std::ifstream::in);

	if (!ifs.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	char c;
	int tile_x = 0;
	int tile_y = 0;
	Tile* new_tile;
	while (ifs.get(c)) {
		switch (c)
		{
		case '.':
			// Set proper tile
			new_tile = new Tile(
				tile_x,
				tile_y,
				this->TILE_WIDTH_,
				this->TILE_HEIGHT_,
				0,
				0
			);
			this->SetTile(tile_x, tile_y, new_tile);
			tile_y++;
			break;
		case '\n':
			tile_x++;
			tile_y = 0;
			break;
		}
	}

}

Tile* Map::GetTile(int x, int y) {
	return this->tile_matrix_[x][y];
}

void Map::SetTile(int x, int y, Tile* tile) {
	this->tile_matrix_[x][y] = tile;
}

void Map::Render(SDL_Renderer* renderer) {
	for (size_t i = 0; i < this->size_x_; i++)
	{
		for (size_t j = 0; j < this->size_y_; j++)
		{
			// Render tile
			this->tile_matrix_[i][j]->render(renderer);
		}
	}
}
