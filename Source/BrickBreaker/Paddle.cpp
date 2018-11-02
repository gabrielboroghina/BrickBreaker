#include "Paddle.h"

#include "Core/GPU/Mesh.h"
#include "BasicMeshes.h"

namespace Object
{
Paddle::Paddle() : yTop(44), length(150), xCenter(0), yCenter(40)
{
	mesh = CreateRect("paddle", glm::vec3(0, 0, 0), 8, length, glm::vec3(1, 1, 1), true);
}

Paddle::~Paddle() {}


void Paddle::Render() {}

void Paddle::CenterAtXPos(float x)
{
	xCenter = x;
}
}
