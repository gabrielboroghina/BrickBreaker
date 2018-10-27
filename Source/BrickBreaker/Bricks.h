#pragma once
#include "Core/GPU/Mesh.h"
#include <unordered_map>

class Bricks
{
public:
	int numBrickLines, numBrickCols;
	std::unordered_map<int, Mesh *> bricks;
	glm::mat3x3 translateMatrix;
	float brickHeight, brickWidth, f;

	Bricks(int numBrickLines, int numBrickCols, float winHeight, float winWidth);
	~Bricks();
	std::tuple<float, float, float, float> GetBrickBounds(int i, int j);
};
