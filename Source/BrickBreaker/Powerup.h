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
	std::vector<glm::vec3> GetCorners();

private:
	glm::mat3 translateMatrix, rotateMatrix;
	const float duration = 2;
	float velocity;
};
