#pragma once

#include <glm/mat3x3.hpp>

class Object2D
{
public:
	glm::mat3 translateMatrix, scaleMatrix;

	Object2D();
	virtual ~Object2D();
	glm::mat3 GetTransformMatrix();
};
