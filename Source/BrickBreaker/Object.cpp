#include "Object.h"

Object::Object() {}

Object::~Object() {}

glm::mat3 Object::GetTransformMatrix()
{
	return translateMatrix * scaleMatrix;
}
