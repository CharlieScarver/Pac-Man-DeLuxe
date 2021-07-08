#pragma once
#include "GameObject.h"

class Unit : public GameObject {
protected:
	Unit(float x, float y, float width, float height);
	~Unit();

	void ManageMovement();
	void ManageAnimation();

public:
	int current_tile_row_;
	int current_tile_col_;

	virtual void update() override;
	virtual void render(SDL_Renderer* renderer) override;
};
