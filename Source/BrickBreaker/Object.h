#pragma once
#include <glm/mat3x3.hpp>

class Object
{
public:
	glm::mat3 transformMatrix, scaleMatrix;

	Object();
	virtual ~Object();
};
