#pragma once

#include "Core/GPU/Mesh.h"

class Powerup
{
public:
	Powerup() : ttl(0) {}

	virtual ~Powerup() {}

	virtual void Update(float deltaTime) = 0;
	bool IsActive();
	virtual void Enable();

protected:
	float ttl;
};

class BottomWall : public Powerup
{
public:
	static constexpr float yTop = 15, yPos = 10;

	BottomWall();
	~BottomWall();
	void Update(float deltaTime) override;
	void Enable() override;

private:
	Mesh *mesh;
	glm::mat3x3 transformMatrix;
};
