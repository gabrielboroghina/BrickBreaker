#include "Powerups.h"

#include "BasicMeshes.h"
#include "PowerupManager.h"
#include "Transform2D.h"

bool Powerup::IsActive() const { return ttl > 0; }

void Powerup::Enable() { ttl = 4; }

BottomWall::BottomWall()
{
	mesh = CreateRect("bottomWall", glm::vec3(PowerupManager::viewportSize.x / 2, yPos, 1),
	                  yTop - yPos, PowerupManager::viewportSize.x, glm::vec3(0.06f, 0.58f, 0.8f), true);

	transformMatrix = glm::mat3(1);
}

BottomWall::~BottomWall()
{
	delete mesh;
}

void BottomWall::Update(float deltaTime)
{
	ttl -= deltaTime;
	if (ttl <= 0)
		PowerupManager::activePowerupsMeshes.erase(PowerupManager::activePowerupsMeshes.find(mesh));
}

void BottomWall::Enable()
{
	Powerup::Enable();
	PowerupManager::activePowerupsMeshes[mesh] = transformMatrix;
}


Shooter::Shooter() : isActive(false) { }

Shooter::~Shooter()
{
	while (!recycledBullets.empty())
		delete recycledBullets.top();

	for (auto &bullet : activeBullets)
		delete bullet.first;
}

void Shooter::Update(float deltaTime)
{
	ttl -= deltaTime;

	if (ttl <= 0) {
		for (auto &bullet : activeBullets)
			recycledBullets.push(bullet.first);
		activeBullets.clear();
	}

	for (auto &bullet : activeBullets)
		bullet.second *= Transform2D::Translate(0, deltaTime * 600);
}

std::unordered_map<Mesh *, glm::mat3x3> &Shooter::GetBulletMeshes()
{
	return activeBullets;
}

void Shooter::Fire(float x)
{
	if (!recycledBullets.empty()) {
		activeBullets[recycledBullets.top()] = Transform2D::Translate(x, 0);
		recycledBullets.pop();
	}
	else {
		// spawn a new bullet
		activeBullets[CreateRect("bullet", glm::vec3(0, /* yPaddle */30, 1), 8, 3, glm::vec3(0.27f, 0.73f, 0.1f), true)]
			= Transform2D::Translate(x, 0);
	}
}

void Shooter::Enable()
{
	Powerup::Enable();
	isActive = true;
}

void Shooter::KillBullet(Mesh *bullet)
{
	activeBullets.erase(bullet);
	recycledBullets.push(bullet);
}

bool Shooter::HasEnded()
{
	if (isActive && ttl <= 0) {
		isActive = false;
		return true;
	}
	return false;
}
