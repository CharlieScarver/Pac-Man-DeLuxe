#include <vector>
#include <queue>
#include <unordered_map>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Ghost.h"
#include "Map.h"

// Original ghost speed is 1.46 pixels per frame (at 60 fps)
const float Ghost::default_velocity_ = 1.46f * 1.08f;

// Frightened velocity is 50% of normal velocity
const float Ghost::frightened_velocity_ = Ghost::default_velocity_ * 0.5f;

const float Ghost::turn_radius_ = 2.5f;

Ghost::Ghost(float x, float y, Map* map, GhostType type)
	: Unit(x, y, Unit::render_width_, Unit::render_height_, map), scatter_timer_(), chase_timer_(), frightened_timer_()
{
	this->type_ = type;

	switch (this->type_)
	{
		case GhostType::BLINKY:
			this->spritesheet_position_ = Vector2(Ghost::blinky_spritesheet_x_, Ghost::blinky_spritesheet_y_);

			// Initial movement
			this->direction_ = Direction::RIGHT;
			break;
		case GhostType::PINKY:
			this->spritesheet_position_ = Vector2(Ghost::pinky_spritesheet_x_, Ghost::pinky_spritesheet_y_);
			
			// Initial movement
			this->direction_ = Direction::LEFT;
			break;
		case GhostType::INKY:
			this->spritesheet_position_ = Vector2(Ghost::inky_spritesheet_x_, Ghost::inky_spritesheet_y_);

			// Initial movement
			this->direction_ = Direction::RIGHT;
			break;
		case GhostType::CLYDE:
			this->spritesheet_position_ = Vector2(Ghost::clyde_spritesheet_x_, Ghost::clyde_spritesheet_y_);
			
			// Initial movement
			this->direction_ = Direction::LEFT;
			break;
	}

	this->animation_frames_count_ = Ghost::default_animation_frames_;
	this->animation_delay_ = Ghost::default_animation_delay_;
	
	this->velocity_x_ = Ghost::default_velocity_;
	this->velocity_y_ = Ghost::default_velocity_;

	this->is_turning_ = false;

	this->is_eaten_ = false;
	this->previous_mode_ = GhostMode::SCATTER;
	this->mode_ = GhostMode::SCATTER;
	this->scatter_timer_.Start(Ghost::scatter_duration_);

	this->target_tile_ = this->map_->pacman_->current_tile_;

	// Initialize random seed
	srand((unsigned)time(nullptr));

	this->render_debug_ = false;
}

GhostMode Ghost::Mode() {
	return this->mode_;
}

bool Ghost::IsEaten() {
	return this->is_eaten_;
}

Tile* Ghost::GetTargetTile() {
	Tile* new_target_tile = this->target_tile_;

	if (this->is_eaten_) {
		// TODO - respawn tile is const field
		// If the mode is Chase and the ghost is not "eaten" => chase the respawn tile
		new_target_tile = this->map_->GetTile(14, 14);
	}
	else if (this->mode_ == GhostMode::CHASE && !this->is_eaten_) {
		// If the mode is Chase and the ghost is not "eaten" => chase a Pac-Man-dependent tile based on ghost type
		if (this->type_ == GhostType::BLINKY) {
			// Blinky targets Pac-Man's current tile
			new_target_tile = this->map_->pacman_->current_tile_;
		}
		else if (this->type_ == GhostType::PINKY) {
			// Pinky targets four tiles ahead of Pac-Man's current tile in the corresponding direction

			// Get the tile that is four tiles ahead of Pac-Man using his orientation as the direction
			new_target_tile = this->map_->GetNthTileInDirection(this->map_->pacman_->current_tile_, Utilities::GetDirectionFromOrientation(this->map_->pacman_->orientation_), 4);
		}
		else if (this->type_ == GhostType::INKY) {
			// Inky mirrors Blinky's position using two tiles ahead of Pac-Man as the origin point.
			// He takes the tile distance and the direction between Blinky and two tiles ahead of Pac-Man and doubles the distance in the same direction. The resulting tile is his target.

			// Get the tile that is two tiles ahead of Pac-Man using his orientation as the direction
			Tile* two_tiles_ahead = this->map_->GetNthTileInDirection(this->map_->pacman_->current_tile_, Utilities::GetDirectionFromOrientation(this->map_->pacman_->orientation_), 2);

			// If two tiles ahead of Pac-Man is outside of the map => use Pac-Man's tile
			if (two_tiles_ahead == nullptr) {
				two_tiles_ahead = this->map_->pacman_->current_tile_;
			}

			// Get the tile distance and the direction between Blinky and two tiles ahead of Pac-Man
			// Note - Returned direction is where tile2 (Blinky) is in regard to tile1 (two tiles ahead of Pac-Man)
			int distance_x = this->map_->GetHorizontalTileDistanceBetweenTwoTiles(two_tiles_ahead, this->map_->blinky_->current_tile_);
			int distance_y = this->map_->GetVerticalTileDistanceBetweenTwoTiles(two_tiles_ahead, this->map_->blinky_->current_tile_);

			// The new target is this much tiles away from "two tiles ahead of Pac-Man" using the same direction (might be outside of the map in which case = nullptr)
			new_target_tile = this->map_->GetTile(two_tiles_ahead->map_x_ + distance_x, two_tiles_ahead->map_y_ + distance_y);
		}
		else if (this->type_ == GhostType::CLYDE) {
			// If Pac-Man's current tile is more than 8 tiles away => Clyde targets Pac'Man's current tile
			if (this->map_->GetTileDistanceBetweenTwoTiles(this->current_tile_, this->map_->pacman_->current_tile_) >= 8) {
				new_target_tile = this->map_->pacman_->current_tile_;
			}
			else {
				// Otherwise Clyde targets the bottom left
				new_target_tile = this->map_->GetTile(1, 32);
			}
		}
	}
	else if (this->mode_ == GhostMode::SCATTER) {
		// If the mode is Scatter => chase the scatter tile based on ghost type
		if (this->type_ == GhostType::BLINKY) {
			// Blinky targets the top right corner
			new_target_tile = this->map_->GetTile(26, 4);
		}
		else if (this->type_ == GhostType::PINKY) {
			// Pinky targets the top left corner
			new_target_tile = this->map_->GetTile(1, 4);
		}
		else if (this->type_ == GhostType::INKY) {
			// Inky targets the bottom right
			new_target_tile = this->map_->GetTile(26, 32);
		}
		else {
			// Clyde targets the bottom left
			new_target_tile = this->map_->GetTile(1, 32);
		}
	}
	else if (this->mode_ == GhostMode::FRIGHTENED) {
		// If frightened target the respawn tile (just to have a target, it doesn't matter)
		new_target_tile = this->map_->GetTile(14, 14);
	}

	// If the new target is invalid => target Pac-Man
	if (new_target_tile == nullptr || new_target_tile->is_solid_) {
		new_target_tile = this->map_->pacman_->current_tile_;
	}

	return new_target_tile;
}

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
	if (this->render_debug_) {
		// Clear the data from the last algorithm run (reallocation not guaranteed but also not needed)
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

			if (this->render_debug_) {
				// When a new layer starts => add a new vector for it
				this->visited_layers_.push_back(std::vector<Tile*>());
			}
		}

		if (this->render_debug_) {
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

	// Make sure ghosts don't get stuck
	if (next_turn_direction == Direction::NONE) {
		next_turn_direction = GetRandomValidDirection();
	}

	return next_turn_direction;
}

Direction Ghost::GetRandomValidDirection() {
	// Get neighbouring tiles
	std::vector<Tile*> neighbour_tiles = this->map_->GetNeighbourTiles(this->current_tile_);
	// Vector to save possible directions
	std::vector<Direction> possible_directions;

	// Loop through neighbouring tiles
	for (int i = 0; i < neighbour_tiles.size(); i++)
	{
		Direction neighbour_direction = this->map_->GetDirectionBetweenNeighbourTiles(this->current_tile_, neighbour_tiles[i]);

		// If tile is not solid and is not behind the ghost => turn in that direction
		if (!neighbour_tiles[i]->is_solid_ && neighbour_direction != Utilities::GetOppositeDirection(this->direction_)) {
			possible_directions.push_back(neighbour_direction);
		}
	}

	// Random number between 0 and (size - 1)
	int random_number = possible_directions.size() == 1
		? 0
		: rand() % (possible_directions.size() - 1);

	return possible_directions[random_number];
}

void Ghost::AI(float delta_time) {
	// Get target tile based on mode and ghost type
	this->target_tile_ = this->GetTargetTile();

	// Manage movement:	

	// If on a turn tile and not currently turning => take the turn
	if (this->current_tile_->is_turn_tile_ && !this->is_turning_) {
		// Get ghost render center
		Vector2F render_center = Utilities::GetCenterPointOfRectangle(this->render_position_, this->render_size_);

		// Get current tile center
		Vector2F current_tile_center = Utilities::GetCenterPointOfRectangle(this->current_tile_->render_position_, this->current_tile_->render_size_);

		// Check if ghost is in the turn interval
		bool center_x_in_turn_interval = render_center.x_ >= current_tile_center.x_ - Ghost::turn_radius_ && render_center.x_ <= current_tile_center.x_ + Ghost::turn_radius_;
		bool center_y_in_turn_interval = render_center.y_ >= current_tile_center.y_ - Ghost::turn_radius_ && render_center.y_ <= current_tile_center.y_ + Ghost::turn_radius_;

		// If the ghost is in the turn interval (around the center of the tile) => take the turn
		if (center_x_in_turn_interval && center_y_in_turn_interval) {
			// Turning in progress
			this->is_turning_ = true;

			// Position the ghost in the center of the tile
			this->SetCenterToTileCenter(this->current_tile_);

			// If the ghost is "eaten" or the mode is Chase or Scatter => follow the normal ghost movement rules
			if (this->is_eaten_ || this->mode_ == GhostMode::CHASE || this->mode_ == GhostMode::SCATTER) {
				// Set the turn direction
				this->direction_ = this->GetNextTurnDirection();
			}
			else {
				// If the mode is Frightened => pick directions at random on every turn
				this->direction_ = this->GetRandomValidDirection();
			}
		}
	}
	else if (!this->current_tile_->is_turn_tile_) {
		// Turning completed
		this->is_turning_ = false;
	}

	// When ghost reaches the respawn tile after being eaten => revive it
	if (this->current_tile_->id_ == this->map_->GetTile(14, 14)->id_ && this->is_eaten_) {
		this->is_eaten_ = false;

		// Restart the chase mode
		this->ChangeMode(GhostMode::CHASE);

		// Get target tile based on mode and ghost type
		this->target_tile_ = this->GetTargetTile();
	}

	// Timers:

	// If the ghost has been "eaten" => don't update the timers
	if (!this->is_eaten_) {
		switch (this->mode_)
		{
			case GhostMode::SCATTER:
				this->scatter_timer_.Update(delta_time);
				if (this->scatter_timer_.HasCompleted()) {
					this->ChangeMode(GhostMode::CHASE);
				}
				break;
			case GhostMode::CHASE:
				this->chase_timer_.Update(delta_time);
				if (this->chase_timer_.HasCompleted()) {
					this->ChangeMode(GhostMode::SCATTER);
				}
				break;
			case GhostMode::FRIGHTENED:
				this->frightened_timer_.Update(delta_time);
				if (this->frightened_timer_.HasCompleted()) {
					this->ChangeMode(this->previous_mode_);
				}
				break;
		}
	}
}

void Ghost::ChangeMode(GhostMode new_mode) {
	// If ghost was frightened until now => update the sprites and velocity
	if (this->mode_ == GhostMode::FRIGHTENED && new_mode != GhostMode::FRIGHTENED) {
		switch (this->type_)	
		{
			case GhostType::BLINKY:
				this->spritesheet_position_.x_ = Ghost::blinky_spritesheet_x_;
				this->spritesheet_position_.y_ = Ghost::blinky_spritesheet_y_;
				break;
			case GhostType::PINKY:
				this->spritesheet_position_.x_ = Ghost::pinky_spritesheet_x_;
				this->spritesheet_position_.y_ = Ghost::pinky_spritesheet_y_;
				break;
			case GhostType::INKY:
				this->spritesheet_position_.x_ = Ghost::inky_spritesheet_x_;
				this->spritesheet_position_.y_ = Ghost::inky_spritesheet_y_;
				break;
			case GhostType::CLYDE:
				this->spritesheet_position_.x_ = Ghost::clyde_spritesheet_x_;
				this->spritesheet_position_.y_ = Ghost::clyde_spritesheet_y_;
				break;
			}

		// Update ghost velocity
		this->velocity_x_ = Ghost::default_velocity_;
		this->velocity_y_ = Ghost::default_velocity_;
	}

	switch (new_mode)
	{
		case GhostMode::SCATTER:
			// If the current mode is Frigtherend and the previous mode was Scatter and the scatter timer didn't complete (was paused) => unpause it
			if (this->mode_ == GhostMode::FRIGHTENED && this->previous_mode_ == GhostMode::SCATTER && !this->scatter_timer_.HasCompleted()) {
				this->scatter_timer_.Unpause();
			}
			else {
				// If not => start a new scatter timer
				this->scatter_timer_.Start(Ghost::scatter_duration_);
			}
			break;
		case GhostMode::CHASE:
			// If the current mode is Frigtherend and the previous mode was Chase and the chase timer didn't complete (was paused) => unpause it
			if (this->mode_ == GhostMode::FRIGHTENED && this->previous_mode_ == GhostMode::CHASE && !this->chase_timer_.HasCompleted()) {
				this->chase_timer_.Unpause();
				// But what if eaten??
			}
			else {
				// If not => start a new chase timer
				this->chase_timer_.Start(Ghost::chase_duration_);
			}
			break;
		case GhostMode::FRIGHTENED:
			this->frightened_timer_.Start(Ghost::frightened_duration_);
			
			// Pause the other timers (only one of them is running)
			this->scatter_timer_.Pause();
			this->chase_timer_.Pause();

			// Update the sprites
			this->spritesheet_position_.x_ = Ghost::frightened_spritesheet_x_;
			this->spritesheet_position_.y_ = Ghost::frightened_spritesheet_y_;

			// Update ghost velocity
			this->velocity_x_ = Ghost::frightened_velocity_;
			this->velocity_y_ = Ghost::frightened_velocity_;
			break;
	}

	// If current mode is Frightened and the new mode is Frighetened => don't update the previous state (because the pre-fright state will be lost)
	// (Pac-Man ate a second enerizer before the first one ran out)
	if (!(this->mode_ == GhostMode::FRIGHTENED && new_mode == GhostMode::FRIGHTENED)) {
		// Save previous mode
		this->previous_mode_ = this->mode_;
	}

	// If the current mode and the new mode are different => reverse the direction of the ghost to signify the change
	if (this->mode_ != new_mode) {
		// Reverse ghosts' direction of movement when changing modes
		this->ReverseDirection();
	}

	// Update the current mode
	this->mode_ = new_mode;
}

void Ghost::ReverseDirection() {
	// Reverse ghosts' direction of movement when changing modes
	this->direction_ = Utilities::GetOppositeDirection(this->direction_);

	// If the ghost is on a turn tile => calculate the shortest path and the next direction again to avoid getting stuck after reversing
	if (this->current_tile_->is_turn_tile_) {
		this->is_turning_ = false;
	}
}

void Ghost::Frighten() {
	if (!this->is_eaten_) {
		this->ChangeMode(GhostMode::FRIGHTENED);
	}
}

void Ghost::GetEaten() {
	this->is_eaten_ = true;

	// Chase the respawn tile
	//this->ChangeMode(GhostMode::CHASE);
	this->target_tile_ = this->GetTargetTile();

	// Restore default ghost velocity
	this->velocity_x_ = Ghost::default_velocity_;
	this->velocity_y_ = Ghost::default_velocity_;

	// Update sprite (no sprite)
	this->spritesheet_position_.x_ = Ghost::eaten_spritesheet_x_;
	this->spritesheet_position_.y_ = Ghost::eaten_spritesheet_y_;
}

void Ghost::Update(float delta_time, const Uint8* keyboard_state) {
	if (keyboard_state[SDL_SCANCODE_G]) {
		return;
	}

	Unit::Update(delta_time, keyboard_state);

	this->AI(delta_time);
}

void Ghost::Render(SDL_Renderer* renderer, AssetLoader* asset_loader) {
	// Body spritesheet rect
	SDL_Rect spritesheet_rect;
	spritesheet_rect.x = this->spritesheet_position_.x_ + (this->current_animation_frame_ * this->sprite_size_.x_);
	spritesheet_rect.y = this->spritesheet_position_.y_;
	spritesheet_rect.w = this->sprite_size_.x_;
	spritesheet_rect.h = this->sprite_size_.y_;

	SDL_FRect render_rect;
	render_rect.x = this->render_position_.x_;
	render_rect.y = this->render_position_.y_;
	render_rect.w = this->render_size_.x_;
	render_rect.h = this->render_size_.y_;

	// Render the body
	SDL_RenderCopyF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect);

	if (this->render_debug_) {
		// Draw sprite box
		SDL_SetRenderDrawColor(renderer, 50, 25, 200, 0);
		SDL_RenderDrawRectF(renderer, &render_rect);
	}

	// Render the eyes when not frightened or when "eaten"
	if (this->mode_ != GhostMode::FRIGHTENED || this->is_eaten_) {
		// Render the eyes according to the orientation
		switch (this->orientation_)
		{
		case Orientation::UP:
			// Up eyes spritesheet rect
			spritesheet_rect.x = Ghost::eyes_up_spritesheet_x_;
			spritesheet_rect.y = Ghost::eyes_up_spritesheet_y_;
			break;
		case Orientation::DOWN:
			// Down eyes spritesheet rect
			spritesheet_rect.x = Ghost::eyes_down_spritesheet_x_;
			spritesheet_rect.y = Ghost::eyes_down_spritesheet_y_;
			break;
		case Orientation::LEFT:
			// Left eyes spritesheet rect
			spritesheet_rect.x = Ghost::eyes_left_spritesheet_x_;
			spritesheet_rect.y = Ghost::eyes_left_spritesheet_y_;
			break;
		case Orientation::RIGHT:
			// Right eyes spritesheet rect
			spritesheet_rect.x = Ghost::eyes_right_spritesheet_x_;
			spritesheet_rect.y = Ghost::eyes_right_spritesheet_y_;
			break;
		}

		// Render the eyes
		SDL_RenderCopyF(renderer, asset_loader->units_spritesheet_, &spritesheet_rect, &render_rect);
	}

	// Render the searching algorithm (only for Blinky to save resources and make it more readable)
	if (this->render_debug_ && this->type_ == GhostType::BLINKY) {
		for (int layer = 0; layer < this->visited_layers_.size(); layer++)
		{
			for (int tile = 0; tile < this->visited_layers_[layer].size(); tile++)
			{
				// Draw target tile rect
				SDL_FRect target_tile_rect;
				target_tile_rect.x = (float)(this->visited_layers_[layer][tile]->map_x_ * Tile::render_width_);
				target_tile_rect.y = (float)(this->visited_layers_[layer][tile]->map_y_ * Tile::render_height_);
				target_tile_rect.w = (float)Tile::render_width_;
				target_tile_rect.h = (float)Tile::render_height_;

				// Pretty colors that depend on the layer
				SDL_SetRenderDrawColor(renderer, layer * 10, 200, 255 - (layer * 10), 0);
				SDL_RenderDrawRectF(renderer, &target_tile_rect);
			}
		}
	}

	// Render shortest path chosen by the ghost
	if (this->render_debug_) {
		// Draw last calculated path
		for (int i = (int)this->reversed_path_to_target_.size() - 1; i >= 0; i--)
		{
			Tile* tile = this->reversed_path_to_target_[i];

			SDL_FRect tile_rect;
			tile_rect.x = (float)(tile->map_x_ * Tile::render_width_);
			tile_rect.y = (float)(tile->map_y_ * Tile::render_height_);
			tile_rect.w = (float)Tile::render_width_;
			tile_rect.h = (float)Tile::render_height_;

			if (this->type_ == GhostType::BLINKY) {
				SDL_SetRenderDrawColor(renderer, 200, 25, 25, 0);
			}
			else if (this->type_ == GhostType::PINKY) {
				SDL_SetRenderDrawColor(renderer, 255, 175, 250, 0);
			}
			else if (this->type_ == GhostType::INKY) {
				SDL_SetRenderDrawColor(renderer, 25, 200, 200, 0);
			}
			else if (this->type_ == GhostType::CLYDE) {
				SDL_SetRenderDrawColor(renderer, 175, 100, 25, 0);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 200, 200, 25, 0);
			}
			SDL_RenderDrawRectF(renderer, &tile_rect);
		}

		// Draw target tile rect
		SDL_FRect target_tile_rect;
		target_tile_rect.x = (float)(this->target_tile_->map_x_ * Tile::render_width_);
		target_tile_rect.y = (float)(this->target_tile_->map_y_ * Tile::render_height_);
		target_tile_rect.w = (float)Tile::render_width_;
		target_tile_rect.h = (float)Tile::render_height_;

		SDL_SetRenderDrawColor(renderer, 25, 200, 25, 0);
		SDL_RenderDrawRectF(renderer, &target_tile_rect);
	}
}
