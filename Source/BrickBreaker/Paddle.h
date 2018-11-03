#pragma once

#include "Core/GPU/Mesh.h"

namespace Object
{
/**
 * The paddle is used by the user to keep the ball inside the game area
 * and to redirect it on another direction.
 */
class Paddle
{
public:
	float yTop, length, xCenter, yCenter;
	Mesh *mesh;

	Paddle();
	~Paddle();
	void CenterAtXPos(float x); // change the position of the paddle's center
};
}
