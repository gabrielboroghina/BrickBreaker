#pragma once

#include "Core/GPU/Mesh.h"

class Walls
{
public:
	const float wallWidth = 15;
	float verticalHeight;
	Mesh *meshLeft, *meshRight, *meshTop;

	Walls(float initialX, float initialY);
	~Walls();

private:
	const glm::vec3 color = glm::vec3(186.0 / 255, 54.0 / 255, 11.0 / 255);
};
