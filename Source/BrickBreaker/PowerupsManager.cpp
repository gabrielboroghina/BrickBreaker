#include "PowerupsManager.h"
#include <ctime>
#include "BasicMeshes.h"

PowerupsManager::PowerupsManager(float viewportX)
{
	srand(time(NULL));
	bottomWall = CreateRect("bottomWall", glm::vec3(viewportX / 2, bottomWallY, 1), bottomWallYTop - bottomWallY,
	                        viewportX, glm::vec3(0.06f, 0.58f, 0.8f), true);
}

PowerupsManager::~PowerupsManager() {}

void PowerupsManager::MaySpawn(float x, float y)
{
	if (rand() % 2 == 1) {
		// decide if we'll spawn a powerup
		PowerupType powerupType = static_cast<PowerupType>(rand() % 1);
		powerup.push_back(new Powerup(powerupType, x, y));
	}
}

void PowerupsManager::Update(float deltaTime)
{
	for (auto p : powerup) {
		p->Update(deltaTime);

		if (p->mesh == nullptr)
			EndPowerup(p);
	}

	if (activePowerupsMeshes[bottomWall]) {
		bottomWallTTL -= deltaTime;
		if (bottomWallTTL <= 0)
			activePowerupsMeshes[bottomWall] = false;
	}
}

void PowerupsManager::EndPowerup(Powerup *p)
{
	delete p;
	powerup.erase(std::find(powerup.begin(), powerup.end(), p));
}

bool PowerupsManager::isPowerupActive(PowerupType type)
{
	switch (type) {
		case BOTTOM_WALL:
			return activePowerupsMeshes[bottomWall];
	}
	return false;
}

void PowerupsManager::EnablePowerup(Powerup *powerup)
{
	switch (powerup->type) {
		case PowerupType::BOTTOM_WALL:
			// create bottom wall
			activePowerupsMeshes[bottomWall] = true;
			bottomWallTTL = 4; // seconds
			break;
	}

	EndPowerup(powerup);
}

std::vector<Powerup *> &PowerupsManager::GetPowerups()
{
	return powerup;
}
