#pragma once

#include "Core/GPU/Mesh.h"
#include <unordered_map>

namespace Object
{
/**
 * This object consists of a matrix of identical bricks.
 */
class Bricks
{
public:
	int numBrickLines, numBrickCols;
	std::unordered_map<int, Mesh *> brick;
	glm::mat3x3 translateMatrix;
	float brickHeight, brickWidth;

	Bricks(int numBrickLines, int numBrickCols, glm::vec2 viewportSize);
	~Bricks();

	void Update(float deltaTime);
	void Blast(int brickIndex);
	bool WasBlasted(int brickIndex);
	glm::mat3x3 GetTransformMatrix(int brickIndex);
	std::tuple<float, float, float, float> GetBrickBounds(int i, int j);

private:
	float fSpaceBrick; // the proportion space_size / brick_size
	std::unordered_map<int, float> scaleFactor;
};
}
