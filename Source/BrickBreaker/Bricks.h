#pragma once

#include "Core/GPU/Mesh.h"
#include <unordered_map>

class Bricks
{
public:
	int numBrickLines, numBrickCols;
	std::unordered_map<int, Mesh *> brick;
	glm::mat3x3 translateMatrix;
	float brickHeight, brickWidth;

	Bricks(int numBrickLines, int numBrickCols, float winHeight, float winWidth);
	~Bricks();

	void Update(float deltaTime);
	void Blast(int brickIndex);
	bool WasBlasted(int brickIndex);
	glm::mat3x3 GetTransformMatrix(int brickIndex);
	std::tuple<float, float, float, float> GetBrickBounds(int i, int j);

private:
	float fSpaceBrick;
	std::unordered_map<int, float> scaleFactor;
};
