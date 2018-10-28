#pragma once

#include "Core/GPU/Mesh.h"

class Walls
{
public:
	const float wallWidth = 15;
	float verticalHeight, topWallWidth;
	Mesh *meshLeft, *meshRight, *meshTop;

	Walls(float initialX, float initialY);
	~Walls();

private:
	const glm::vec3 color = glm::vec3(0.6f, 0.08f, 0);
};
