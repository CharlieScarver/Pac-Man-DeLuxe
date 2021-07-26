#pragma once
#include <vector>
#include "AssetLoader.h"
#include "Tile.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Direction.h"
#include "Item.h"

#define MAP_WIDTH_IN_TILES 28
#define MAP_HEIGHT_IN_TILES 36
#define MAP_OFFSET_X 0
#define MAP_OFFSET_Y 0

class Map final {
private:
	char* file_name_;
	Tile* tile_matrix_[MAP_WIDTH_IN_TILES][MAP_HEIGHT_IN_TILES];

public:
	/// <summary>
	/// The asset loader used to get the loaded assets (like spritesheets). 
	/// Used to load, get and unload assets.
	/// </summary>
	AssetLoader* asset_loader_;

	bool collision_occured_;
	bool no_more_items_;

	PacMan* pacman_;
	std::vector<Ghost*> ghosts_;
	std::vector<Item*> items_;

	Map(AssetLoader* asset_loader);
	~Map();

	void LoadMapFromFile(const char* file_name);
	Tile* GetTile(int x, int y);
	void SetTile(int x, int y, Tile* tile);

	bool DetermineIfTileIsTurn(int x, int y);

	int GetTileDistanceBetweenTwoTiles(Tile* tile1, Tile* tile2);
	Tile* GetNextTileInDirection(Tile* source_tile, Direction direction);

	/// <summary>
	/// Returns a vector with the neighbouring tiles (four or less if on the edge of the map).
	/// </summary>
	std::vector<Tile*> GetNeighbourTiles(Tile* origin_tile);

	/// <summary>
	/// Returns the direction that has to be used to travel from a given tile to another given neighbouring tile.
	/// </summary>
	Direction GetDirectionBetweenNeighbourTiles(Tile* source, Tile* destination);

	/// <summary>
	/// Frighten the ghosts.
	/// Used when Pac-Man eats an energizer.
	/// </summary>
	void FrightenGhosts();

	void Update(float delta_time, const Uint8* keyboard_state);
	void Render(SDL_Renderer* renderer, AssetLoader* asset_loader);
};
