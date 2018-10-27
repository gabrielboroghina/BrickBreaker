#pragma once
#include <glm/mat3x3.hpp>

class Object
{
public:
	glm::mat3 transformMatrix;

	Object();
	virtual ~Object();
};
