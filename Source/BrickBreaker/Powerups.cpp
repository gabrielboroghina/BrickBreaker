#include "Powerups.h"

#include "BasicMeshes.h"
#include "PowerupManager.h"
#include "Transform2D.h"

Powerup::Powerup() : ttl(0), isActive(false) { }

bool Powerup::IsActive() const { return ttl > 0; }

void Powerup::Enable()
{
	isActive = true;
	ttl = 4; // set time to live countdown
}

bool Powerup::HasEnded()
{
	// return true only the first time after a powerup expiration
	if (isActive && ttl <= 0) {
		isActive = false;
		return true;
	}
	return false;
}


BottomWall::BottomWall()
{
	float viewportWidth = PowerupManager::GetInstance()->viewportSize.x;
	mesh = CreateRect("bottomWall", glm::vec3(viewportWidth / 2, yPos, 1), yTop - yPos, viewportWidth,
	                  glm::vec3(0.06f, 0.58f, 0.8f), true);

	transformMatrix = glm::mat3(1);
}

BottomWall::~BottomWall()
{
	delete mesh;
}

void BottomWall::Update(float deltaTime)
{
	ttl -= deltaTime;

	PowerupManager *powerupManager = PowerupManager::GetInstance();
	if (ttl <= 0) // powerup expired
		powerupManager->activePowerupsMeshes.erase(powerupManager->activePowerupsMeshes.find(mesh));
}

void BottomWall::Enable()
{
	Powerup::Enable();
	// insert bottom wall mesh into activePowerupsMeshes in order to be rendered in the Update method.
	PowerupManager::GetInstance()->activePowerupsMeshes[mesh] = transformMatrix;
}


Shooter::Shooter() {}

Shooter::~Shooter()
{
	// delete all bullets
	while (!recycledBullets.empty())
		delete recycledBullets.top();

	for (auto &bullet : activeBullets)
		delete bullet.first;
}

void Shooter::Update(float deltaTime)
{
	ttl -= deltaTime;

	if (ttl <= 0) {
		// recycle all bullets
		for (auto &bullet : activeBullets)
			recycledBullets.push(bullet.first);
		activeBullets.clear();
	}

	for (auto &bullet : activeBullets)
		bullet.second *= Transform2D::Translate(0, deltaTime * 600); // shift upwards the bullet
}

std::unordered_map<Mesh *, glm::mat3> &Shooter::GetBulletMeshes()
{
	return activeBullets;
}

void Shooter::Fire(float x)
{
	if (!recycledBullets.empty()) {
		// use a recycled bullet
		activeBullets[recycledBullets.top()] = Transform2D::Translate(x, 0);
		recycledBullets.pop();
	}
	else {
		// spawn a new bullet
		activeBullets[CreateRect("bullet", glm::vec3(0, /* yPaddle */30, 1), 8, 3, glm::vec3(0.27f, 0.73f, 0.1f), true)]
			= Transform2D::Translate(x, 0);
	}
}

void Shooter::KillBullet(Mesh *bullet)
{
	// recycle bullet
	activeBullets.erase(bullet);
	recycledBullets.push(bullet);
}


FatBall::FatBall() {}

FatBall::~FatBall() {}

void FatBall::Update(float deltaTime)
{
	ttl -= deltaTime;
}
