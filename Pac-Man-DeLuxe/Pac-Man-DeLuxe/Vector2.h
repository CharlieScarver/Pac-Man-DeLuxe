#pragma once

struct Vector2 {
	float x_;
	float y_;

	Vector2() {
		this->x_ = 0;
		this->y_ = 0;
	}
	Vector2(float x, float y) {
		this->x_ = x;
		this->y_ = y;
	}
	
	static Vector2 FindCenterPointOfRectangle(Vector2 position, Vector2 size) {
		return Vector2(position.x_ + size.x_ / 2.0f, position.y_ + size.y_ / 2.0f);
	}

	static bool IsValueInInterval(float value, float interval_center, float interval_radius) {
		return value >= interval_center - interval_radius && value <= interval_center + interval_radius;
	}
};
