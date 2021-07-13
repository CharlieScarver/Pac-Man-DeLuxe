#include <iostream>
#include <fstream>
#include "Map.h"

Map::Map() {
	this->file_name_ = nullptr;
}

Map::~Map() {
	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			delete this->tile_matrix_[i][j];
		}
	}
}

void Map::LoadMapFromFile(const char* file_name) {
	std::ifstream ifs;

	// TODO: Prefix file name with Assets/
	ifs.open(file_name, std::ifstream::in);

	std::cout << "Map file opened." << std::endl;

	if (!ifs.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	char c;
	// Columns = width
	int tile_x = 0;
	// Rows = height
	int tile_y = 0;
	while (ifs.get(c)) {
		switch (c)
		{
		case '.':
			// Set proper tile and increase the column counter
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, EMPTY));
			tile_x++;
			break;
		case '=':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_HORIZONTAL_TOP));
			tile_x++;
			break;
		case '-': // \u2550 = 45 somehow
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_HORIZONTAL_BOTTOM));
			tile_x++;
			break;
		case '|':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_VERTICAL_LEFT));
			tile_x++;
			break;
		case '!':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_VERTICAL_RIGHT));
			tile_x++;
			break;
		case ']':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_ROUNDED_TOP_LEFT));
			tile_x++;
			break;
		case '[':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_ROUNDED_TOP_RIGHT));
			tile_x++;
			break;
		case ')':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_ROUNDED_BOTTOM_LEFT));
			tile_x++;
			break;
		case '(': // \u2554 = 43 somehow
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, WALL_ROUNDED_BOTTOM_RIGHT));
			tile_x++;
			break;
		case '\n':
			// When we reach a newline set the column to zero and increase the row counter
			tile_x = 0;
			tile_y++;
			break;
		}
	}

	std::cout << "Map loaded." << std::endl;
}

Tile* Map::GetTile(int x, int y) {
	return this->tile_matrix_[x][y];
}

void Map::SetTile(int x, int y, Tile* tile) {
	this->tile_matrix_[x][y] = tile;
}

void Map::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// The built-in array uses signed subscripts.
	// Given the known problems with unsigned and signed/unsigned mixtures, better stick to (signed) integers of a sufficient size.
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex

	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			// Render tile
			this->tile_matrix_[i][j]->Render(renderer, asset_loader);
		}
	}
}
