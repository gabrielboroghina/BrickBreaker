#include "Object2D.h"

Object2D::Object2D() {}

Object2D::~Object2D() {}

glm::mat3 Object2D::GetTransformMatrix()
{
	return translateMatrix * scaleMatrix;
}
