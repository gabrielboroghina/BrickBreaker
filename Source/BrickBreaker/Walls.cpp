#include "Walls.h"
#include <glm/detail/type_vec3.hpp>
#include "BasicMeshes.h"

Walls::Walls(float initialX, float initialY)
{
	verticalHeight = initialY * 0.9;
	topWallWidth = initialX;
	meshLeft = CreateRect("wall", glm::vec3(0, 0, 0), verticalHeight, wallWidth, color, true);
	meshRight = CreateRect("wall", glm::vec3(0, 0, 0), verticalHeight, wallWidth, color, true);
	meshTop = CreateRect("wall", glm::vec3(0, 0, 0), wallWidth, topWallWidth, color, true);
}

Walls::~Walls()
{
	delete meshLeft;
	delete meshRight;
	delete meshTop;
}
