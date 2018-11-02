#include "PowerupManager.h"

#include <ctime>
#include "BasicMeshes.h"

PowerupManager *PowerupManager::instance = nullptr; // singleton instance

std::unordered_map<Mesh *, glm::mat3x3> PowerupManager::activePowerupsMeshes;
glm::vec2 PowerupManager::viewportSize;
std::unordered_map<PowerupType, Powerup *> PowerupManager::powerups;

PowerupManager::PowerupManager()
{
	srand(time(NULL));

	// TODO duplicated enum values
	powerups = {
		{BOTTOM_WALL, nullptr},
		{SHOOTER, nullptr},
		{FAT_BALL, nullptr}
	};
}

PowerupManager::~PowerupManager() {}

PowerupManager *PowerupManager::GetInstance()
{
	if (!instance)
		instance = new PowerupManager();

	return instance;
}

void PowerupManager::SetViewportSize(glm::vec2 size) { viewportSize = size; }

void PowerupManager::MaySpawn(float x, float y)
{
	if (rand() % 3 == 1) {
		// decide if we'll spawn a powerup ticket
		PowerupType powerupType = static_cast<PowerupType>(rand() % 3);
		powerupTickets.push_back(new PowerupTicket(powerupType, x, y));

		// lazy initialization for powerup
		if (!powerups[powerupType])
			switch (powerupType) {
				case BOTTOM_WALL:
					powerups[powerupType] = new BottomWall();
					break;
				case SHOOTER:
					powerups[powerupType] = new Shooter();
					break;
				case FAT_BALL:
					powerups[powerupType] = new FatBall();
					break;
			}
	}
}

void PowerupManager::UpdatePowerups(float deltaTime)
{
	for (auto p : powerupTickets) {
		p->Update(deltaTime);

		if (p->mesh == nullptr)
			KillPowerupTicket(p);
	}

	for (auto &p : powerups)
		if (p.second && p.second->IsActive())
			p.second->Update(deltaTime);
}

Powerup *PowerupManager::GetPowerup(PowerupType type)
{
	return powerups[type];
}

void PowerupManager::KillPowerupTicket(PowerupTicket *p)
{
	delete p;
	powerupTickets.erase(std::find(powerupTickets.begin(), powerupTickets.end(), p));
}

bool PowerupManager::IsPowerupActive(PowerupType type)
{
	return powerups[type] && powerups[type]->IsActive();
}

void PowerupManager::EnablePowerup(PowerupTicket *powerupTicket)
{
	powerups[powerupTicket->type]->Enable();
	KillPowerupTicket(powerupTicket);
}

std::vector<PowerupTicket *> &PowerupManager::GetTickets()
{
	return powerupTickets;
}
