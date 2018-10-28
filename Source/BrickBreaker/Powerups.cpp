#include "Powerups.h"

#include "BasicMeshes.h"
#include "PowerupManager.h"

bool Powerup::IsActive() { return ttl > 0; }

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
