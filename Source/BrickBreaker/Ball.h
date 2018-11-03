#pragma once

#include "Core/GPU/Mesh.h"
#include "Object2D.h"

namespace Object
{
class Ball : public Object2D
{
public:
	Mesh *mesh;
	bool isAttached; // checks if ball is attached to the paddle

	float radius;
	const float stdRadius = 10;
	float vx, vy; // the ball's velocity on Ox and Oy axis

	Ball(float initialX, float initialY);
	~Ball();
	void Attach(); // attach the ball to the paddle
	void Detach(); // detach the ball from the paddle
	void Scale(float scaleX, float scaleY);
	void Resize(float resizeFactor);
	void Update(float deltaTime, float xCursor, float yBottom);
	void ReflectX();
	void ReflectY();
	void ReflectAngled(float cos);

private:
	const float velocity = 450;
};
}
