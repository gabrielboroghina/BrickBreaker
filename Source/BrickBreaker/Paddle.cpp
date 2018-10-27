#include "Paddle.h"

#include <GL/glew.h>
#include "Core/GPU/Mesh.h"
#include "BasicMeshes.h"

namespace Object2D
{
Paddle::Paddle() : yTop(34), length(150), xCenter(0), yCenter(30)
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
