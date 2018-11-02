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
	const float stdRadius = 10;
	float vx, vy; // the ball's velocity

	Ball(float initialX, float initialY);
	~Ball();
	void Attach();
	void Detach();
	void Scale(float scaleX, float scaleY);
	void Resize(float newRadius);
	void Update(float deltaTime, float xCursor, float yPos);
	void ReflectX();
	void ReflectY();
	void ReflectAngled(float cos);

private:
	const float velocity = 450;
};
}
