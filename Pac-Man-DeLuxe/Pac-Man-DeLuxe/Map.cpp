﻿#include <iostream>
#include <fstream>
#include <cmath>
#include "Map.h"

Map::Map(AssetLoader* asset_loader) {
	this->asset_loader_ = asset_loader;

	this->file_name_ = nullptr;
	this->pacman_ = nullptr;

	this->collision_occured_ = false;
	this->no_more_items_ = false;

	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			this->tile_matrix_[i][j] = nullptr;
		}
	}
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

	// Free the ghosts
	for (int i = 0; i < this->ghosts_.size(); i++)
	{
		delete this->ghosts_[i];
	}

	// Free the items
	for (int i = 0; i < this->items_.size(); i++)
	{
		delete this->items_[i];
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

	Tile* tile;
	Item* item;

	// Parse the map file
	while (ifs.get(c)) {
		switch (c)
		{
		case '.':
			// Create the tile
			tile = new Tile(tile_x, tile_y, TileType::EMPTY);

			// Create item and add it to the items vector
			item = new Item(tile, ItemType::PELLET);
			this->items_.push_back(item);

			// Set the contained item
			tile->contained_item_ = item;

			// Set proper tile and increase the column counter
			this->SetTile(tile_x, tile_y, tile);
			tile_x++;
			break;
		case '*':
			// Create the tile
			tile = new Tile(tile_x, tile_y, TileType::EMPTY);

			// Create item and add it to the items vector
			item = new Item(tile, ItemType::ENERGIZER);
			this->items_.push_back(item);

			// Set the contained item
			tile->contained_item_ = item;

			// Set proper tile and increase the column counter
			this->SetTile(tile_x, tile_y, tile);
			tile_x++;
			break;
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
	this->pacman_ = new PacMan(pacman_x, pacman_y, this);

	// Create ghosts	
	this->ghosts_.push_back(new Ghost(312, 324, this, GhostType::BLINKY));
	this->ghosts_.push_back(new Ghost(312, 324, this, GhostType::PINKY));
	this->ghosts_.push_back(new Ghost(312, 324, this, GhostType::INKY));
	this->ghosts_.push_back(new Ghost(312, 324, this, GhostType::CLYDE));

	std::cout << "Map loaded." << std::endl;
}

Tile* Map::GetTile(int x, int y) {
	if ((x < 0 || x >= MAP_WIDTH_IN_TILES) || (y < 0 || y >= MAP_HEIGHT_IN_TILES)) {
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

int Map::GetTileDistanceBetweenTwoTiles(Tile* tile1, Tile* tile2) {
	int x_distance = abs(tile1->map_x_ - tile2->map_x_);
	int y_distance = abs(tile1->map_y_ - tile2->map_y_);

	return x_distance + y_distance;
}

Tile* Map::GetNextTileInDirection(Tile* source_tile, Direction direction) {
	if (source_tile == nullptr) {
		return nullptr;
	}

	switch (direction)
	{
		case Direction::UP:
			return this->GetTile(source_tile->map_x_, source_tile->map_y_ - 1);
		case Direction::DOWN:
			return this->GetTile(source_tile->map_x_, source_tile->map_y_ + 1);
		case Direction::LEFT:
			return this->GetTile(source_tile->map_x_ - 1, source_tile->map_y_);
		case Direction::RIGHT:
			return this->GetTile(source_tile->map_x_ + 1, source_tile->map_y_);
		case Direction::NONE:
			return source_tile;
		default:
			return nullptr;
	}
}

std::vector<Tile*> Map::GetNeighbourTiles(Tile* origin_tile) {
	// Tile preference order is UP, LEFT, DOWN, RIGHT (most to least preferred)
	std::vector<Tile*> storage;

	Tile* neighbour_tile = this->GetNextTileInDirection(origin_tile, Direction::UP);
	if (neighbour_tile != nullptr) {
		storage.push_back(neighbour_tile);
	}

	neighbour_tile = this->GetNextTileInDirection(origin_tile, Direction::LEFT);
	if (neighbour_tile != nullptr) {
		storage.push_back(neighbour_tile);
	}

	neighbour_tile = this->GetNextTileInDirection(origin_tile, Direction::DOWN);
	if (neighbour_tile != nullptr) {
		storage.push_back(neighbour_tile);
	}

	neighbour_tile = this->GetNextTileInDirection(origin_tile, Direction::RIGHT);
	if (neighbour_tile != nullptr) {
		storage.push_back(neighbour_tile);
	}

	return storage;
}

Direction Map::GetDirectionBetweenNeighbourTiles(Tile* source, Tile* destination) {
	// Compare the map indices of the tiles to find the direction
	if (source->map_x_ - destination->map_x_ == 1) {
		return Direction::LEFT;
	}
	else if (source->map_x_ - destination->map_x_ == -1) {
		return Direction::RIGHT;
	}
	else if (source->map_y_ - destination->map_y_ == 1) {
		return Direction::UP;
	}
	else if (source->map_y_ - destination->map_y_ == -1) {
		return Direction::DOWN;
	}
	
	// The tiles are not neighbouring
	return Direction::NONE;
}

void Map::FrightenGhosts() {
	for (int j = 0; j < this->ghosts_.size(); j++)
	{
		this->ghosts_[j]->Frighten();
	}
}

void Map::Update(float delta_time, const Uint8* keyboard_state) {
	// Update Pac-Man
	this->pacman_->Update(delta_time, keyboard_state);

	// Update the ghosts
	for (int i = 0; i < this->ghosts_.size(); i++)
	{
		this->ghosts_[i]->Update(delta_time, keyboard_state);

		// Check for collision between Pac-Man and ghost
		if (this->pacman_->current_tile_->id_ == this->ghosts_[i]->current_tile_->id_) {
			
			// If ghost is "eaten" => do nothing (pass through each other)
			if (this->ghosts_[i]->IsEaten()) {
				continue;
			}
			else if (this->ghosts_[i]->Mode() == GhostMode::FRIGHTENED && !this->ghosts_[i]->IsEaten()) {
				// If ghost is frightened (and not "eaten") => Pac-Man eats it
				
				// Calculate score gained = (200 * (2 ^ ghosts_already_eaten))
				int gained_score = 200;
				for (int j = 0; j < this->ghosts_.size(); j++)
				{
					// Double the score for every other eaten ghost
					if (this->ghosts_[j]->IsEaten()) {
						gained_score *= 2;
					}
				}
				this->pacman_->GainScore(200);

				// Mark the ghost as eaten
				this->ghosts_[i]->GetEaten();
			}
			else {
				// If ghost is not frightened and is not "eaten" (all other cases) => collision occured => Game over
				this->collision_occured_ = true;
			}
		}
	}

	// Update items
	for (int i = 0; i < this->items_.size(); i++)
	{
		this->items_[i]->Update(delta_time, keyboard_state);
	}
}

void Map::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// The built-in array uses signed subscripts.
	// Given the known problems with unsigned and signed/unsigned mixtures, better stick to (signed) integers of a sufficient size.
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex

	this->no_more_items_ = true;
	// Render tiles
	for (int i = 0; i < MAP_WIDTH_IN_TILES; i++)
	{
		for (int j = 0; j < MAP_HEIGHT_IN_TILES; j++)
		{
			// Render tile
			this->tile_matrix_[i][j]->Render(renderer, asset_loader);

			if (this->tile_matrix_[i][j]->contained_item_ != nullptr) {
				this->no_more_items_ = false;
			}
		}
	}

	// Render Pac-Man
	this->pacman_->Render(renderer, asset_loader);

	// Render the ghosts
	for (int i = 0; i < this->ghosts_.size(); i++)
	{
		this->ghosts_[i]->Render(renderer, asset_loader);
	}
}
