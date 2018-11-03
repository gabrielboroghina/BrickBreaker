#pragma once

#include "Core/GPU/Mesh.h"
#include <stack>
#include <unordered_map>

enum PowerupType
{
	BOTTOM_WALL,
	SHOOTER,
	FAT_BALL
};

/**
 * Abstract class containing the general behavior of a powerup
 */
class Powerup
{
public:
	Powerup();
	virtual ~Powerup() {}

	virtual void Update(float deltaTime) = 0;
	bool IsActive() const;
	virtual void Enable();
	bool HasEnded(); // one-time check for the powerup timeout

protected:
	float ttl; // time to live
	bool isActive; // used by the HasEnded method
};

class BottomWall : public Powerup
{
public:
	static constexpr float yTop = 15, yPos = 10; // yPos - center's y coordinate

	BottomWall();
	~BottomWall();
	void Update(float deltaTime) override;
	void Enable() override;

private:
	Mesh *mesh;
	glm::mat3 transformMatrix;
};

class Shooter : public Powerup
{
public:
	Shooter();
	~Shooter();
	void Update(float deltaTime) override;
	void Fire(float x);
	void KillBullet(Mesh *bullet); // recycle bullet
	std::unordered_map<Mesh *, glm::mat3> &GetBulletMeshes();

private:
	std::unordered_map<Mesh *, glm::mat3> activeBullets;
	std::stack<Mesh *> recycledBullets;
};

class FatBall : public Powerup
{
public:
	FatBall();
	~FatBall();
	void Update(float deltaTime) override;
};
