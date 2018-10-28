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

	fSpaceBrick = 0.3;
	brickHeight = height / ((fSpaceBrick + 1) * numBrickLines - fSpaceBrick);
	brickWidth = width / ((fSpaceBrick + 1) * numBrickCols - fSpaceBrick);

	for (i = 0; i < numBrickLines; i++)
		for (j = 0; j < numBrickCols; j++) {
			float x = brickWidth * ((fSpaceBrick + 1) * j + 0.5);
			float y = brickHeight * ((fSpaceBrick + 1) * i + 0.5);
			brick[i * numBrickCols + j] = CreateRect("brick", glm::vec3(x, y, 0), brickHeight, brickWidth,
			                                         color, true);
		}

	translateMatrix = Transform2D::Translate(winWidth * (-fXSpace + 1) / 2, 3 * winHeight * (-fYSpace + 1) / 4);
}

Bricks::~Bricks() {}

void Bricks::Update(float deltaTime)
{
	for (auto it = brick.begin(); it != brick.end();)
		if (scaleFactor.count(it->first)) {
			scaleFactor[it->first] -= deltaTime * 2;

			if (scaleFactor[it->first] <= 0.01f) {
				// brick is completely destroyed
				scaleFactor.erase(it->first);
				auto itAux = it++;
				brick.erase(itAux);
			}
			else ++it;
		}
		else ++it;
}

void Bricks::Blast(int brickIndex)
{
	scaleFactor[brickIndex] = 1;
}

bool Bricks::WasBlasted(int brickIndex)
{
	return scaleFactor.count(brickIndex) != 0;
}

glm::mat3x3 Bricks::GetTransformMatrix(int brickIndex)
{
	float xCenter = brickWidth * ((fSpaceBrick + 1) * (brickIndex % numBrickCols) + 0.5);
	float yCenter = brickHeight * ((fSpaceBrick + 1) * (brickIndex / numBrickCols) + 0.5);

	if (scaleFactor.count(brickIndex))
		return translateMatrix * Transform2D::Translate(xCenter, yCenter) *
			Transform2D::Scale(scaleFactor[brickIndex], scaleFactor[brickIndex]) *
			Transform2D::Translate(-xCenter, -yCenter);

	return translateMatrix;
}

std::tuple<float, float, float, float> Bricks::GetBrickBounds(int i, int j)
{
	float xMin = brickWidth * (fSpaceBrick + 1) * j;
	float yMin = brickHeight * (fSpaceBrick + 1) * i;

	glm::vec2 cornerMin = translateMatrix * glm::vec3(xMin, yMin, 1);
	glm::vec2 cornerMax = translateMatrix * glm::vec3(xMin + brickWidth, yMin + brickHeight, 1);

	return std::make_tuple(cornerMin.x, cornerMax.x, cornerMin.y, cornerMax.y);
}
