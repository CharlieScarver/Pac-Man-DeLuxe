#pragma once
#include "Vector2F.h"
#include "Direction.h"
#include "Orientation.h"

class Utilities {
private:
	Utilities() = default;
	~Utilities() = default;

public:
	/// <summary>
	/// Returns the center point of a rectangle represented by two vectors - one for the position of the top left vertex (x, y) and one for the size (width, height).
	/// </summary>
	static Vector2F GetCenterPointOfRectangle(Vector2F position, Vector2F size);

	/// <summary>
	/// Returns if a given value in a given interval represented by a center point and an interval radius.
	/// </summary>
	static bool IsValueInInterval(float value, float interval_center, float interval_radius);

	/// <summary>
	/// Returns the opposite direction of the given one.
	/// </summary>
	static Direction GetOppositeDirection(Direction direction);

	/// <summary>
	/// Returns a direction based on an orientation.
	/// Can be used to avoid receiving an undesired Direction::NONE for a stationary target.
	/// </summary>
	static Direction GetDirectionFromOrientation(Orientation orientation);
};
