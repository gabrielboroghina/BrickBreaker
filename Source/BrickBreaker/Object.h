#pragma once
#include <glm/mat3x3.hpp>

class Object
{
public:
	glm::mat3 translateMatrix, scaleMatrix;

	Object();
	virtual ~Object();
	glm::mat3 GetTransformMatrix();
};
