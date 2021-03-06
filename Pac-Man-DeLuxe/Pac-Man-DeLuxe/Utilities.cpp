#include "Utilities.h"

Vector2F Utilities::GetCenterPointOfRectangle(Vector2F position, Vector2F size) {
	return Vector2F(position.x_ + size.x_ / 2.0f, position.y_ + size.y_ / 2.0f);
}

bool Utilities::IsValueInInterval(float value, float interval_center, float interval_radius) {
	return value >= interval_center - interval_radius && value <= interval_center + interval_radius;
}

Direction Utilities::GetOppositeDirection(Direction direction) {
	switch (direction)
	{
		case Direction::NONE:
			return Direction::NONE;
		case Direction::UP:
			return Direction::DOWN;
		case Direction::DOWN:
			return Direction::UP;
		case Direction::LEFT:
			return Direction::RIGHT;
		case Direction::RIGHT:
			return Direction::LEFT;
		default:
			return Direction::NONE;
	}
}

Direction Utilities::GetDirectionFromOrientation(Orientation orientation) {
	switch (orientation)
	{
		case Orientation::UP:
			return Direction::UP;
		case Orientation::DOWN:
			return Direction::DOWN;
		case Orientation::LEFT:
			return Direction::LEFT;
		case Orientation::RIGHT:
			return Direction::RIGHT;
		default:
			return Direction::LEFT;
	}
}
