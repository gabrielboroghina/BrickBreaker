#include "Walls.h"
#include <glm/detail/type_vec3.hpp>
#include "BasicMeshes.h"

namespace Object
{
Walls::Walls(glm::vec2 viewportSize)
{
	verticalHeight = viewportSize.y * 0.85f; // leave a gap at the bottom of the side walls
	topWallWidth = viewportSize.x;

	meshLeft = CreateRect("wall", glm::vec3(0, 0, 0), verticalHeight, thickness, color, true);
	meshRight = CreateRect("wall", glm::vec3(0, 0, 0), verticalHeight, thickness, color, true);
	meshTop = CreateRect("wall", glm::vec3(0, 0, 0), thickness, topWallWidth, color, true);
}

Walls::~Walls()
{
	delete meshLeft;
	delete meshRight;
	delete meshTop;
}
}
