#pragma once

#include "Core/GPU/Mesh.h"
#include <stack>
#include <unordered_map>

enum PowerupType
{
	BOTTOM_WALL,
	SHOOTER
};

class Powerup
{
public:
	Powerup() : ttl(0) {}

	virtual ~Powerup() {}

	virtual void Update(float deltaTime) = 0;
	bool IsActive() const;
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

class Shooter : public Powerup
{
public:
	Shooter();
	~Shooter();
	void Update(float deltaTime) override;
	void Fire();
	void Enable() override;
	std::unordered_map<Mesh *, glm::mat3x3> &GetBulletMeshes();

private:
	std::unordered_map<Mesh *, glm::mat3x3> activeBullets;
	std::stack<std::pair<Mesh *, glm::mat3x3>> recycledBullets;
};
