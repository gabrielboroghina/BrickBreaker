#include "Bricks.h"
#include "BasicMeshes.h"
#include "Transform2D.h"
#include <tuple>


Bricks::Bricks(int numBrickLines, int numBrickCols, float winHeight, float winWidth) : numBrickLines(numBrickLines),
                                                                                       numBrickCols(numBrickCols)
{
	int i, j;
	const float fXSpace = 0.6f, fYSpace = 0.4f;

	glm::vec3 color = glm::vec3(0.7f, 0.26f, 0.1f);
	float height = fYSpace * winHeight;
	float width = fXSpace * winWidth;

	f = 0.3;
	brickHeight = height / ((f + 1) * numBrickLines - f);
	brickWidth = width / ((f + 1) * numBrickCols - f);

	for (i = 0; i < numBrickLines; i++)
		for (j = 0; j < numBrickCols; j++) {
			float x = brickWidth * ((f + 1) * j + 0.5);
			float y = brickHeight * ((f + 1) * i + 0.5);
			bricks[i * numBrickCols + j] = CreateRect("brick", glm::vec3(x, y, 0), brickHeight, brickWidth,
			                                          color, true);
		}

	translateMatrix = Transform2D::Translate(winWidth * (-fXSpace + 1) / 2, 3 * winHeight * (-fYSpace + 1) / 4);
}


Bricks::~Bricks() {}

std::tuple<float, float, float, float> Bricks::GetBrickBounds(int i, int j)
{
	float xMin = brickWidth * (f + 1) * j;
	float yMin = brickHeight * (f + 1) * i;

	glm::vec2 cornerMin = translateMatrix * glm::vec3(xMin, yMin, 1);
	glm::vec2 cornerMax = translateMatrix * glm::vec3(xMin + brickWidth, yMin + brickHeight, 1);

	return std::make_tuple(cornerMin.x, cornerMax.x, cornerMin.y, cornerMax.y);
}
