#pragma once

#include <glm/mat3x3.hpp>
#include "Core/GPU/Mesh.h"

enum PowerupType
{
	BOTTOM_WALL
};

class Powerup
{
public:
	PowerupType type;
	Mesh *mesh;

	Powerup(PowerupType type, float x, float y);
	~Powerup();
	void Update(float deltaTime);
	glm::mat3x3 GetTransformMatrix();

private:
	glm::mat3 translateMatrix, rotateMatrix;
	const float duration = 2;
	float velocity;
};
