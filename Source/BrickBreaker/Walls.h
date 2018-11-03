#pragma once

#include "Core/GPU/Mesh.h"

namespace Object
{
/**
 * Object containing the three walls placed on the left, right and top of the game area.
 */
class Walls
{
public:
	static constexpr float thickness = 15;
	float verticalHeight, topWallWidth;
	Mesh *meshLeft, *meshRight, *meshTop;

	Walls(glm::vec2 viewportSize);
	~Walls();

private:
	const glm::vec3 color = glm::vec3(0.48f, 0.08f, 0.23f);
};
}
