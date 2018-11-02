#include "Ball.h"

#include "BasicMeshes.h"
#include "Transform2D.h"

namespace Object2D
{
Ball::Ball(float initialX, float yBottom) : isAttached(true)
{
	radius = stdRadius;
	mesh = CreateCircle(radius, glm::vec3(1, 1, 1), glm::vec3(0.26f, 0.31f, 0.19f));
	translateMatrix = Transform2D::Translate(initialX, yBottom + radius);
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

void Ball::Scale(float scaleX, float scaleY)
{
	scaleMatrix = Transform2D::Scale(scaleX, scaleY);
}

void Ball::Resize(float resizeFactor)
{
	radius = stdRadius * resizeFactor;
	Scale(resizeFactor, resizeFactor);
}

void Ball::Update(float deltaTime, float xCursor, float yBottom)
{
	if (isAttached)
		translateMatrix = Transform2D::Translate(xCursor, yBottom + radius);
	else
		translateMatrix = Transform2D::Translate(deltaTime * vx, deltaTime * vy) * translateMatrix;
}

void Ball::ReflectX() { vx = -vx; }

void Ball::ReflectY() { vy = -vy; }

void Ball::ReflectAngled(float cos)
{
	vx = velocity * cos;
	vy = velocity * sqrt(1 - cos * cos); // velocity * sin
}
}
