#pragma once

#include "Powerups.h"
#include <glm/mat3x3.hpp>
#include "Core/GPU/Mesh.h"

class PowerupTicket
{
public:
	PowerupType type;
	Mesh *mesh;

	PowerupTicket(PowerupType type, float x, float y);
	~PowerupTicket();
	void Update(float deltaTime);
	glm::mat3 GetTransformMatrix() const;
	std::vector<glm::vec3> GetCorners() const; // get coordinates of the square's corners

private:
	glm::mat3 translateMatrix, rotateMatrix;
	float velocity;
};
