#include "Ball.h"

#include "BasicMeshes.h"
#include "Transform2D.h"

namespace Object2D
{
Ball::Ball(float initialX, float initialY) : radius(9), isAttached(true)
{
	mesh = CreateCircle(radius);
	transformMatrix = Transform2D::Translate(initialX, initialY);
	scaleMatrix = glm::mat3(1);
}

Ball::~Ball() { delete mesh; }

void Ball::Attach() { isAttached = true; }

void Ball::Detach()
{
	isAttached = false;

	// set initial velocity vector
	vx = 0;
	vy = velocity;
}

void Ball::Scale(float scaleFactor)
{
	scaleMatrix = Transform2D::Scale(scaleFactor, scaleFactor);
	transformMatrix *= scaleMatrix;
}

void Ball::Update(float deltaTime, float xCursor, float yPos)
{
	if (isAttached)
		transformMatrix = Transform2D::Translate(xCursor, yPos) * scaleMatrix;
	else
		transformMatrix = Transform2D::Translate(deltaTime * vx, deltaTime * vy) * transformMatrix;
}

void Ball::ReflectX() { vx = -vx; }

void Ball::ReflectY() { vy = -vy; }

void Ball::ReflectAngled(float cos)
{
	vx = velocity * cos;
	vy = velocity * sqrt(1 - cos * cos); // velocity * sin
}
}
