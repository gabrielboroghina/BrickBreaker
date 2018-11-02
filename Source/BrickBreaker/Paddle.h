#pragma once

#include "Core/GPU/Mesh.h"

namespace Object
{
class Paddle
{
public:
	float yTop, length, xCenter, yCenter;
	Mesh *mesh;

	Paddle();
	~Paddle();
	void Render();
	void CenterAtXPos(float x);
};
}
