#include <vector>
#include <queue>
#include <map>
#include "Ghost.h"
#include "Map.h"

const float Ghost::ghost_default_velocity_ = 1.46f * 1.1f;

Ghost::Ghost(float x, float y, Map* map, GhostType ghost_type) : Unit(x, y, Ghost::ghost_render_width_, Ghost::ghost_render_height_, map) {

	this->ghost_type_ = ghost_type;

	switch (this->ghost_type_)
	{
		case GhostType::BLINKY:
			this->spritesheet_position_ = Vector2(Ghost::blinky_spritesheet_x_, Ghost::blinky_spritesheet_y_);

			// Initial movement
			this->direction_ = Direction::LEFT;
			break;
		case GhostType::PINKY:
			this->spritesheet_position_ = Vector2(Ghost::pinky_spritesheet_x_, Ghost::pinky_spritesheet_y_);
			break;
		case GhostType::INKY:
			this->spritesheet_position_ = Vector2(Ghost::inky_spritesheet_x_, Ghost::inky_spritesheet_y_);

			// Initial movement
			this->direction_ = Direction::RIGHT;
			break;
		case GhostType::CLYDE:
			this->spritesheet_position_ = Vector2(Ghost::clyde_spritesheet_x_, Ghost::clyde_spritesheet_y_);
			break;
	}

	this->animation_frames_count_ = Ghost::ghost_animation_frames_;
	this->animation_delay_ = Ghost::ghost_animation_delay_;
	
	this->velocity_x_ = Ghost::ghost_default_velocity_;
	this->velocity_y_ = Ghost::ghost_default_velocity_;

	this->is_turning_ = false;
	this->next_turn_ = Direction::NONE;

	this->target_tile_ = this->map_->pacman_->current_tile_;
}

Ghost::~Ghost() {}

// Breath First Search for the shortest path on the map
std::vector<Tile*> Ghost::FindShortestPath(Tile* start, Tile* goal) {
	// Queue for tiles to visit
	std::queue<Tile*> visit_queue;
	visit_queue.push(start);

	// Array to save which tiles have been visited (using their GameObject ID as index)
	bool visited[MAP_WIDTH_IN_TILES * MAP_HEIGHT_IN_TILES] = { false };
	// Mark start tile as visited
	visited[start->id_] = true;

	// Array to save the previous tile of each visited tile so a path can be reconstructed later (using GameObject ID as index)
	Tile* previous_tiles[MAP_WIDTH_IN_TILES * MAP_HEIGHT_IN_TILES] = { nullptr };

	int visited_layers = 0;
	int tiles_in_next_layer = 2;

	// Loop over the queue until it depletes
	while (visit_queue.size() > 0) {
		Tile* visiting_tile = visit_queue.front();
		visit_queue.pop();

		tiles_in_next_layer--;
		if (tiles_in_next_layer == 0) {
			visited_layers++;
		}

		// If the visited tile is the goal => the path is found
		// Faster compare using GameObject ID
		if (visiting_tile->id_ == goal->id_) {
			break;
		}

		// Find and process neighbouring tiles
		std::vector<Tile*> neighbour_tiles = this->map_->GetNeighbourTiles(visiting_tile);
		tiles_in_next_layer = neighbour_tiles.size();
		for (int i = 0; i < neighbour_tiles.size(); i++)
		{
			// Skip solid and already visited tiles
			if (visited[neighbour_tiles[i]->id_] || neighbour_tiles[i]->is_solid_) {
				tiles_in_next_layer--;
				continue;
			}

			// Only on the first layer
			if (visited_layers == 0) {
				// Skip tiles that require the ghost to turn around (ghosts can't turn around)
				Direction neighbour_direction = this->map_->GetDirectionBetweenNeighbourTiles(start, neighbour_tiles[i]);
				if (neighbour_direction == Utilities::GetOppositeDirection(this->direction_)) {
					tiles_in_next_layer--;
					continue;
				}
			}

			// Push the neighbour tile to the visiting queue
			visit_queue.push(neighbour_tiles[i]);			
			// Mark tile as visited
			visited[neighbour_tiles[i]->id_] = true;
			// Set previous tile
			previous_tiles[neighbour_tiles[i]->id_] = visiting_tile;
		}
	}

	// Reconstruct the path (in reverse)
	std::vector<Tile*> reversed_path;
	for (Tile* tile = previous_tiles[goal->id_]; tile != nullptr; tile = previous_tiles[tile->id_])
	{
		// Push the previous tile to the path
		reversed_path.push_back(tile);
	}

	return reversed_path;
}


void Ghost::AI() {
	if (this->ghost_type_ == GhostType::BLINKY) {
		this->target_tile_ = this->map_->pacman_->current_tile_;
	}
	else {
		this->target_tile_ = this->map_->GetTile(26, 32);
	}

	// If on a turn tile and not currently turning => take the turn
	if (this->current_tile_->is_turn_tile_ && !this->is_turning_) {
		// Get ghost render center
		Vector2 render_center = Utilities::GetCenterPointOfRectangle(this->render_position_, this->render_size_);

		// Get current tile center
		Vector2 current_tile_center = Utilities::GetCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

		bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - UNIT_TURN_PIXEL_RADIUS && render_center.x_ <= current_tile_center.x_ + UNIT_TURN_PIXEL_RADIUS;
		bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - UNIT_TURN_PIXEL_RADIUS && render_center.y_ <= current_tile_center.y_ + UNIT_TURN_PIXEL_RADIUS;

		if (center_x_in_turn_interval && center_y_in_turn_interval) {
			// Turn in progress
			this->SetCenterToTileCenter(this->current_tile_);
			this->direction_ = this->next_turn_;
			this->is_turning_ = true;
			this->next_turn_ = Direction::NONE;
		}
	}
	else {
		// Turn is completed
		this->is_turning_ = false;
	}

	// Get next tile in the facing direction
	Tile* next_tile = this->map_->GetNextTileInDirection(this->current_tile_, this->direction_);

	// If next tile is a turn tile - decide which direction to go in
	if (next_tile->is_turn_tile_ && this->next_turn_ == Direction::NONE) {
		// Find the shortest path to the target tile
		this->reversed_path_to_target_ = this->FindShortestPath(next_tile, this->target_tile_);

		int index_of_the_next_next_tile = 0;
		if (this->reversed_path_to_target_.size() >= 2) {
			// The second to last element marks the next direction of the ghost's next turn
			index_of_the_next_next_tile = this->reversed_path_to_target_.size() - 2;
			// Get the direction of the next turn
			this->next_turn_ = this->map_->GetDirectionBetweenNeighbourTiles(next_tile, this->reversed_path_to_target_[index_of_the_next_next_tile]);
		}
		else {
			// If the ghost is on the target keep going towards the next intersection (after that cycle back)
			std::vector<Tile*> neighbour_tiles = this->map_->GetNeighbourTiles(next_tile);
			for (int i = 0; i < neighbour_tiles.size(); i++)
			{
				Direction neighbour_direction = this->map_->GetDirectionBetweenNeighbourTiles(next_tile, neighbour_tiles[i]);
				if (!neighbour_tiles[i]->is_solid_ && neighbour_direction != Utilities::GetOppositeDirection(this->direction_)) {
					this->next_turn_ = this->map_->GetDirectionBetweenNeighbourTiles(next_tile, neighbour_tiles[i]);
					break;
				}
			}

		}
	}
}

void Ghost::Update(float delta_time, const Uint8* keyboard_state) {
	Unit::Update(delta_time, keyboard_state);

	this->AI();
}

void Ghost::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);

	// Draw last calculated path
	for (int i = this->reversed_path_to_target_.size() - 1; i >= 0; i--)
	{
		Tile* tile = this->reversed_path_to_target_[i];

		SDL_FRect tile_rect;
		tile_rect.x = tile->map_x_ * TILE_RENDER_WIDTH;
		tile_rect.y = tile->map_y_ * TILE_RENDER_HEIGHT;
		tile_rect.w = TILE_RENDER_WIDTH;
		tile_rect.h = TILE_RENDER_HEIGHT;

		if (this->ghost_type_ == GhostType::BLINKY) {
			SDL_SetRenderDrawColor(renderer, 200, 25, 25, 0);
		}
		else if (this->ghost_type_ == GhostType::INKY) {
			SDL_SetRenderDrawColor(renderer, 25, 200, 200, 0);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 200, 200, 25, 0);
		}
		SDL_RenderDrawRectF(renderer, &tile_rect);
	} 

	// Draw target tile rect
	SDL_FRect target_tile_rect;
	target_tile_rect.x = this->target_tile_->map_x_ * TILE_RENDER_WIDTH;
	target_tile_rect.y = this->target_tile_->map_y_ * TILE_RENDER_HEIGHT;
	target_tile_rect.w = TILE_RENDER_WIDTH;
	target_tile_rect.h = TILE_RENDER_HEIGHT;

	SDL_SetRenderDrawColor(renderer, 25, 200, 25, 0);
	SDL_RenderDrawRectF(renderer, &target_tile_rect);
}
