#include "PowerupsManager.h"
#include <ctime>

PowerupsManager::PowerupsManager()
{
	srand(time(NULL));
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

	}
}

void PowerupsManager::EndPowerup(Powerup *p)
{
	delete p;
	powerup.erase(std::find(powerup.begin(), powerup.end(), p));
}

std::vector<Powerup *> &PowerupsManager::GetPowerups()
{
	return powerup;
}
