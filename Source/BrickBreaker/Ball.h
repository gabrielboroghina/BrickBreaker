#pragma once
#include "Core/GPU/Mesh.h"
#include "Object.h"

namespace Object2D
{
class Ball : public Object
{
public:
	Mesh *mesh;
	bool isAttached; // checks if ball is attached to the paddle

	float radius;
	float vx, vy; // the ball's speed

	Ball(float initialX, float initialY);
	~Ball();
	void Attach();
	void Detach();
	void Ball::Update(float deltaTime, float xCursor, float yPos);
	void ReflectX();
	void ReflectY();
	void ReflectAngled(float cos);

private:
	const float speed = 400;
};
}
