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
	static const int letter_sprite_width_ = 8;
	static const int letter_sprite_height_ = 8;
	static const int letter_render_width_ = 24;
	static const int letter_render_height_ = 24;

	static const int win_msg_spritesheet_x_ = 0;
	static const int win_msg_spritesheet_y_ = 0;
	// Message size = number of letters * letter size
	static const int win_msg_sprite_width_ = 8 * Map::letter_sprite_width_;
	static const int win_msg_sprite_height_ = Map::letter_sprite_height_;
	static const float win_msg_render_x_;
	static const float win_msg_render_y_;
	static const int win_msg_render_width_ = 8 * Map::letter_render_width_;
	static const int win_msg_render_height_ = Map::letter_render_width_;

	static const int loss_msg_spritesheet_x_ = 0;
	static const int loss_msg_spritesheet_y_ = 8;
	static const int loss_msg_sprite_width_ = 9 * Map::letter_sprite_width_;
	static const int loss_msg_sprite_height_ = Map::letter_sprite_height_;
	static const float loss_msg_render_x_;
	static const float loss_msg_render_y_;
	static const int loss_msg_render_width_ = 9 * Map::letter_render_width_;
	static const int loss_msg_render_height_ = Map::letter_render_width_;

	static const int score_msg_spritesheet_x_ = 0;
	static const int score_msg_spritesheet_y_ = 16;
	static const int score_msg_sprite_width_ = 9 * Map::letter_sprite_width_;
	static const int score_msg_sprite_height_ = Map::letter_sprite_height_;
	static const float score_msg_render_x_;
	static const float score_msg_render_y_;
	static const int score_msg_render_width_ = 9 * Map::letter_render_width_;
	static const int score_msg_render_height_ = Map::letter_render_width_;

	static const int score_number_digits_ = 6;
	static const float score_number_render_x_;
	static const float score_number_render_y_;

	// Numbers spritesheet coordinates
	static const int zero_spritesheet_x_ = 0;
	static const int zero_spritesheet_y_ = 24;
	static const int one_spritesheet_x_ = 8;
	static const int one_spritesheet_y_ = 24;
	static const int two_spritesheet_x_ = 16;
	static const int two_spritesheet_y_ = 24;
	static const int three_spritesheet_x_ = 24;
	static const int three_spritesheet_y_ = 24;
	static const int four_spritesheet_x_ = 32;
	static const int four_spritesheet_y_ = 24;
	static const int five_spritesheet_x_ = 40;
	static const int five_spritesheet_y_ = 24;
	static const int six_spritesheet_x_ = 48;
	static const int six_spritesheet_y_ = 24;
	static const int seven_spritesheet_x_ = 56;
	static const int seven_spritesheet_y_ = 24;
	static const int eight_spritesheet_x_ = 64;
	static const int eight_spritesheet_y_ = 24;
	static const int nine_spritesheet_x_ = 72;
	static const int nine_spritesheet_y_ = 24;

	char* file_name_;
	Tile* tile_matrix_[MAP_WIDTH_IN_TILES][MAP_HEIGHT_IN_TILES];

	/// <summary>
	/// Renders the numerical part of the score.
	/// </summary>
	void RenderScoreNumber(int score, int digits, SDL_Renderer* renderer, AssetLoader* asset_loader);

public:
	/// <summary>
	/// The asset loader used to get the loaded assets (like spritesheets). 
	/// Used to load, get and unload assets.
	/// </summary>
	AssetLoader* asset_loader_;

	/// <summary>
	/// Shows if a collision between a non-energized Pac-Man and a non-frigthened Ghost has occured (if they're on the same tile).
	/// This is the lose condition.
	/// </summary>
	bool collision_occured_;

	/// <summary>
	/// Shows if all items (pellets, energizers, fruits) have been consumed by Pac-Man.
	/// This is the win condition.
	/// </summary>
	bool no_more_items_;

	PacMan* pacman_;
	Ghost* blinky_;
	std::vector<Ghost*> ghosts_;
	std::vector<Item*> items_;

	Map(AssetLoader* asset_loader);
	~Map();

	void LoadMapFromFile(const char* file_name);
	Tile* GetTile(int x, int y);
	void SetTile(int x, int y, Tile* tile);

	bool DetermineIfTileIsTurn(int x, int y);

	/// <summary>
	/// Returns the horizontal tile distance between two tiles (how much tiles away is the second tile from the first horizontally).
	/// A positive value means tile2 is to the left of tile1 (left) and a negative value means tile2 is to the right of tile1 (right).
	/// </summary>
	int GetHorizontalTileDistanceBetweenTwoTiles(Tile* tile1, Tile* tile2);

	/// <summary>
	/// Returns the vertical tile distance between two tiles (how much tiles away is the second tile from the first vertically).
	/// A positive value means tile2 is lower than tile1 (down) and a negative value means tile2 is highter than tile1 (up).
	/// </summary>
	int GetVerticalTileDistanceBetweenTwoTiles(Tile* tile1, Tile* tile2);

	/// <summary>
	/// Returns the absolute tile distance between two tiles (how much tiles away is the second tile from the first horizontally + vertically).
	/// The distance will always be positive with no indication for direction.
	/// </summary>
	int GetTileDistanceBetweenTwoTiles(Tile* tile1, Tile* tile2);

	/// <summary>
	/// Returns the next tile in a given direction from a given source tile.
	/// </summary>
	Tile* GetNextTileInDirection(Tile* source_tile, Direction direction);

	/// <summary>
	/// Returns the next tile in a given direction from a given source tile.
	/// </summary>
	Tile* GetNthTileInDirection(Tile* source_tile, Direction direction, int steps);

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
