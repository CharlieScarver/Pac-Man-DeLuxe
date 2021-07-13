#pragma once
#include "Unit.h"

class PacMan final : public Unit {
protected:
	PacMan(float x, float y, float width, float height);
	~PacMan();

	void HandleInput();

public:
	int lives_;

	void update() override;
	void render(SDL_Renderer* renderer) override;
};
