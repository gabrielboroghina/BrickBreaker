#pragma once

#include <glm/mat3x3.hpp>
#include "Core/GPU/Mesh.h"

enum PowerupType
{
	BOTTOM_WALL
};

class PowerupTicket
{
public:
	PowerupType type;
	Mesh *mesh;

	PowerupTicket(PowerupType type, float x, float y);
	~PowerupTicket();
	void Update(float deltaTime);
	glm::mat3x3 GetTransformMatrix() const;
	std::vector<glm::vec3> GetCorners() const;

private:
	glm::mat3 translateMatrix, rotateMatrix;
	const float duration = 2;
	float velocity;
};
