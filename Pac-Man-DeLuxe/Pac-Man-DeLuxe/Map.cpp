﻿#include <iostream>
#include <fstream>
#include "Map.h"

Map::Map() {
	this->collision_occured_ = false;
	this->file_name_ = nullptr;
	this->pacman_ = nullptr;

	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			this->tile_matrix_[i][j] = nullptr;
		}
	}

	this->units_ = std::vector<Unit*>();
}

Map::~Map() {
	// Free the tiles
	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			delete this->tile_matrix_[i][j];
		}
	}

	// Free Pac-Man
	delete this->pacman_;

	// Free the other units
	for (int i = 0; i < this->units_.size(); i++)
	{
		delete this->units_[i];
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

	float pacman_x, pacman_y;
	ifs >> pacman_x >> pacman_y;
	
	std::cout << "Spawned Pacman at (" << pacman_x << ", " << pacman_y << ")" << std::endl;

	// Consume the newline
	char c;
	ifs >> std::noskipws >> c;

	// Columns = width
	int tile_x = 0;
	// Rows = height
	int tile_y = 0;
	// Parse the map file
	while (ifs.get(c)) {
		switch (c)
		{
		case '.':
		case ' ':
			// Set proper tile and increase the column counter
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::EMPTY));
			tile_x++;
			break;
		case '=':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_HORIZONTAL_TOP));
			tile_x++;
			break;
		case '-': // \u2550 = 45 somehow
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_HORIZONTAL_BOTTOM));
			tile_x++;
			break;
		case '|':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_VERTICAL_LEFT));
			tile_x++;
			break;
		case '!':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_VERTICAL_RIGHT));
			tile_x++;
			break;
		case ']':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_ROUNDED_TOP_LEFT));
			tile_x++;
			break;
		case '[':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_ROUNDED_TOP_RIGHT));
			tile_x++;
			break;
		case ')':
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_ROUNDED_BOTTOM_LEFT));
			tile_x++;
			break;
		case '(': // \u2554 = 43 somehow
			this->SetTile(tile_x, tile_y, new Tile(tile_x, tile_y, TileType::WALL_ROUNDED_BOTTOM_RIGHT));
			tile_x++;
			break;
		case '\n':
			// When we reach a newline set the column to zero and increase the row counter
			tile_x = 0;
			tile_y++;
			break;
		}
	}

	ifs.close();

	// Set turn tiles - tiles where units can turn
	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			// Tiles on the border can't be turn tiles
			if ((i <= 0 || i >= MAP_WIDTH_IN_TILES - 1) || (j <= 0 || j >= MAP_HEIGHT_IN_TILES - 1)) {
				continue;
			}

			this->tile_matrix_[i][j]->is_turn_tile_ = DetermineIfTileIsTurn(i, j);
		}
	}

	// Create Pac-Man
	this->pacman_ = new PacMan(pacman_x, pacman_y, UNIT_RENDER_WIDTH, UNIT_RENDER_HEIGHT, this);

	// Create ghosts
	this->units_.push_back(new Ghost(312, 180, this, GhostType::BLINKY));
	this->units_.push_back(new Ghost(312, 756, this, GhostType::INKY));

	std::cout << "Map loaded." << std::endl;
}

Tile* Map::GetTile(int x, int y) {
	if ((x < 0 || x >= MAP_WIDTH_IN_TILES) && (y < 0 || y >= MAP_HEIGHT_IN_TILES)) {
		return nullptr;
	}

	return this->tile_matrix_[x][y];
}

void Map::SetTile(int x, int y, Tile* tile) {
	this->tile_matrix_[x][y] = tile;
}

bool Map::DetermineIfTileIsTurn(int x, int y) {
	// If two adjacent tiles on opposite axes are EMPTY => the given tile is on a turn
	if (
		(this->tile_matrix_[x - 1][y]->type_ == TileType::EMPTY &&
			(this->tile_matrix_[x][y - 1]->type_ == TileType::EMPTY || this->tile_matrix_[x][y + 1]->type_ == TileType::EMPTY))
		||
		(this->tile_matrix_[x + 1][y]->type_ == TileType::EMPTY &&
			(this->tile_matrix_[x][y - 1]->type_ == TileType::EMPTY || this->tile_matrix_[x][y + 1]->type_ == TileType::EMPTY))
		) {
		return true;
	}

	return false;
}

void Map::Update(float delta_time, const Uint8* keyboard_state) {
	// Update Pac-Man
	this->pacman_->Update(delta_time, keyboard_state);

	// Update the other units
	for (int i = 0; i < this->units_.size(); i++)
	{
		this->units_[i]->Update(delta_time, keyboard_state);

		// Check for collision between Pac-Man and other units
		if (this->pacman_->current_tile_ == this->units_[i]->current_tile_) {
			this->collision_occured_ = true;
		}
	}
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

	// Render Pac-Man
	this->pacman_->Render(renderer, asset_loader);

	// Rednder the other units
	for (int i = 0; i < this->units_.size(); i++)
	{
		this->units_[i]->Render(renderer, asset_loader);
	}
}
