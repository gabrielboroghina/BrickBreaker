#pragma once

#include "Core/GPU/Mesh.h"

namespace Object
{
class Walls
{
public:
	static constexpr float wallWidth = 15;
	float verticalHeight, topWallWidth;
	Mesh *meshLeft, *meshRight, *meshTop;

	Walls(glm::vec2 viewportSize);
	~Walls();

private:
	const glm::vec3 color = glm::vec3(0.6f, 0.08f, 0);
};
}
