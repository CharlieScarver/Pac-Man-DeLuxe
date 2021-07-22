#include <vector>
#include <queue>
#include <unordered_map>
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
			// Initial movement
			this->direction_ = Direction::RIGHT;
			break;
		case GhostType::INKY:
			this->spritesheet_position_ = Vector2(Ghost::inky_spritesheet_x_, Ghost::inky_spritesheet_y_);

			// Initial movement
			this->direction_ = Direction::LEFT;
			break;
		case GhostType::CLYDE:
			this->spritesheet_position_ = Vector2(Ghost::clyde_spritesheet_x_, Ghost::clyde_spritesheet_y_);
			// Initial movement
			this->direction_ = Direction::RIGHT;
			break;
	}

	this->animation_frames_count_ = Ghost::ghost_animation_frames_;
	this->animation_delay_ = Ghost::ghost_animation_delay_;
	
	this->velocity_x_ = Ghost::ghost_default_velocity_;
	this->velocity_y_ = Ghost::ghost_default_velocity_;

	this->is_turning_ = false;

	this->target_tile_ = this->map_->pacman_->current_tile_;
}

Ghost::~Ghost() {}

std::vector<Tile*> Ghost::FindShortestPath(Tile* start, Tile* goal) {
	// If the start and goal match => return a path with zero steps
	if (start == nullptr || start->id_ == goal->id_)  {
		return std::vector<Tile*>();
	}

	// Queue for tiles to visit
	std::queue<Tile*> visit_queue;
	visit_queue.push(start);

	// Map used to save which tiles have been visited (using their GameObject ID as index)
	std::unordered_map<int, bool> visited_tiles;
	// Mark start tile as visited
	visited_tiles.emplace(start->id_, true);

	// Map used to save the previous tile of each visited tile so a path can be reconstructed later (using GameObject ID as index)
	std::unordered_map<int, Tile*> previous_tiles;

	// Toggle-able logic for visualizing the algorithm
	if (RENDER_GHOSTS_DEBUG) {
		// Clear the data from the last algorithm run
		this->visited_layers_.clear();
		// Add the vector for the first layer
		this->visited_layers_.push_back(std::vector<Tile*>());
	}

	int visited_layers_count = 0;
	size_t tiles_in_next_layer = 2;

	// Loop over the queue until it depletes
	while (visit_queue.size() > 0) {
		Tile* visiting_tile = visit_queue.front();
		visit_queue.pop();

		tiles_in_next_layer--;
		if (tiles_in_next_layer == 0) {
			visited_layers_count++;

			if (RENDER_GHOSTS_DEBUG) {
				// When a new layer starts => add a new vector for it
				this->visited_layers_.push_back(std::vector<Tile*>());
			}
		}

		if (RENDER_GHOSTS_DEBUG) {
			// Add visited tile to the current layer vector
			this->visited_layers_[visited_layers_count].push_back(visiting_tile);
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
			if (visited_tiles.find(neighbour_tiles[i]->id_) != visited_tiles.end() || neighbour_tiles[i]->is_solid_) {
				tiles_in_next_layer--;
				continue;
			}

			// Only on the first layer
			if (visited_layers_count == 0) {
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
			visited_tiles.emplace(neighbour_tiles[i]->id_, true);
			// Save the previous tile
			previous_tiles.emplace(neighbour_tiles[i]->id_, visiting_tile);
		}
	}

	// Reconstruct the path (in reverse)
	std::vector<Tile*> reversed_path;
	Tile* tile = goal;
	while (true)
	{
		// Don't add the start tile to the path
		if (tile->id_ == start->id_) {
			break;
		}

		// Push the previous tile to the path
		reversed_path.push_back(tile);

		// If there isn't a saved previous for this tile => end (path fully constructed)
		if (previous_tiles.find(tile->id_) == previous_tiles.end()) {
			break;
		}

		// If there is a saved previous => get it
		tile = previous_tiles[tile->id_];
	}
	
	return reversed_path;
}

Direction Ghost::GetNextTurnDirection() {
	// Find the shortest path to the target tile
	this->reversed_path_to_target_ = this->FindShortestPath(this->current_tile_, this->target_tile_);

	Direction next_turn_direction = Direction::NONE;
	int path_size = (int)this->reversed_path_to_target_.size();

	if (path_size > 0) {
		// The last element marks the next turn/step (index = size - 1)
		// If the path is only one step long then there's only one element to use for the next turn (index = 0)
		int index_of_the_next_tile = path_size - 1;

		// Get the direction of the next turn (using the next tile)
		next_turn_direction = this->map_->GetDirectionBetweenNeighbourTiles(this->current_tile_, this->reversed_path_to_target_[index_of_the_next_tile]);
	}
	else {
		// If the ghost is on the target tile (path has zero steps) => turn towards an empty neighbour tile (can't turn back though)
		// Tile preference order is UP, LEFT, DOWN, RIGHT (most to least preferred)

		// Get neighbouring tiles
		std::vector<Tile*> neighbour_tiles = this->map_->GetNeighbourTiles(this->current_tile_);
		for (int i = 0; i < neighbour_tiles.size(); i++)
		{
			Direction neighbour_direction = this->map_->GetDirectionBetweenNeighbourTiles(this->current_tile_, neighbour_tiles[i]);

			// If tile is not solid and is not behind the ghost => turn in that direction
			if (!neighbour_tiles[i]->is_solid_ && neighbour_direction != Utilities::GetOppositeDirection(this->direction_)) {
				next_turn_direction = this->map_->GetDirectionBetweenNeighbourTiles(this->current_tile_, neighbour_tiles[i]);
				break;
			}
		}
	}

	return next_turn_direction;
}

void Ghost::AI() {
	if (this->ghost_type_ == GhostType::BLINKY) {
		// Update target tile to be Pac-Man's current tile
		this->target_tile_ = this->map_->pacman_->current_tile_;
	}
	else {
		// Fixed target
		this->target_tile_ = this->map_->GetTile(26, 32);
	}

	// If on a turn tile and not currently turning => take the turn
	if (this->current_tile_->is_turn_tile_ && !this->is_turning_) {
		// Get ghost render center
		Vector2 render_center = Utilities::GetCenterPointOfRectangle(this->render_position_, this->render_size_);

		// Get current tile center
		Vector2 current_tile_center = Utilities::GetCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

		// Check if ghost is in the turn interval
		bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - UNIT_TURN_PIXEL_RADIUS && render_center.x_ <= current_tile_center.x_ + UNIT_TURN_PIXEL_RADIUS;
		bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - UNIT_TURN_PIXEL_RADIUS && render_center.y_ <= current_tile_center.y_ + UNIT_TURN_PIXEL_RADIUS;

		// If the ghost is in the turn interval (around the center of the tile) => take the turn
		if (center_x_in_turn_interval && center_y_in_turn_interval) {
			// Turning in progress
			this->is_turning_ = true;

			// Position the ghost in the center of the tile
			this->SetCenterToTileCenter(this->current_tile_);

			// Set the turn direction
			this->direction_ = this->GetNextTurnDirection();
		}
	}
	else if (!this->current_tile_->is_turn_tile_) {
		// Turning completed
		this->is_turning_ = false;
	}
}

void Ghost::Update(float delta_time, const Uint8* keyboard_state) {
	if (keyboard_state[SDL_SCANCODE_G]) {
		return;
	}

	Unit::Update(delta_time, keyboard_state);

	this->AI();
}

void Ghost::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	Unit::Render(renderer, asset_loader);

	// Render the searching algorithm (only for Blinky to save resources and make it more readable)
	if (RENDER_GHOSTS_DEBUG && this->ghost_type_ == GhostType::BLINKY) {
		for (int layer = 0; layer < this->visited_layers_.size(); layer++)
		{
			for (int tile = 0; tile < this->visited_layers_[layer].size(); tile++)
			{
				// Draw target tile rect
				SDL_FRect target_tile_rect;
				target_tile_rect.x = this->visited_layers_[layer][tile]->map_x_ * TILE_RENDER_WIDTH;
				target_tile_rect.y = this->visited_layers_[layer][tile]->map_y_ * TILE_RENDER_HEIGHT;
				target_tile_rect.w = TILE_RENDER_WIDTH;
				target_tile_rect.h = TILE_RENDER_HEIGHT;

				// Pretty colors that depend on the layer
				SDL_SetRenderDrawColor(renderer, layer * 10, 200, 255 - (layer * 10), 0);
				SDL_RenderDrawRectF(renderer, &target_tile_rect);
			}
		}
	}

	// Render shortest path chosen by the ghost
	if (RENDER_GHOSTS_DEBUG) {
		// Draw last calculated path
		for (int i = (int)this->reversed_path_to_target_.size() - 1; i >= 0; i--)
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
			else if (this->ghost_type_ == GhostType::PINKY) {
				SDL_SetRenderDrawColor(renderer, 255, 175, 250, 0);
			}
			else if (this->ghost_type_ == GhostType::INKY) {
				SDL_SetRenderDrawColor(renderer, 25, 200, 200, 0);
			}
			else if (this->ghost_type_ == GhostType::CLYDE) {
				SDL_SetRenderDrawColor(renderer, 175, 100, 25, 0);
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
}
