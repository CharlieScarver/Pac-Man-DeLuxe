#pragma once
#include "Unit.h"

class PacMan final : public Unit {
protected:
	PacMan(float x, float y, float width, float height);
	~PacMan();

	void HandleInput();

public:
	int lives_;

	void Update() override;
	void Render(SDL_Renderer* renderer) override;
};
