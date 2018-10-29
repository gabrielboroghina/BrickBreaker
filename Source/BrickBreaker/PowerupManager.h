#pragma once

#include "PowerupTicket.h"
#include "Powerups.h"
#include <vector>
#include <unordered_map>

class PowerupManager
{
public:
	static PowerupManager *instance;
	static std::unordered_map<Mesh *, glm::mat3x3> activePowerupsMeshes;

	static PowerupManager *GetInstance();
	~PowerupManager();
	static void SetViewportSize(glm::vec2 size);

	void MaySpawn(float x, float y);
	void UpdatePowerups(float deltaTime);
	void KillPowerupTicket(PowerupTicket *powerupTicket);
	std::vector<PowerupTicket *> &GetTickets();
	void EnablePowerup(PowerupTicket *powerupTicket);
	bool IsPowerupActive(PowerupType type);
	static Powerup *GetPowerup(PowerupType type);

	friend class BottomWall;

private:
	PowerupManager();
	static glm::vec2 viewportSize;
	std::vector<PowerupTicket *> powerupTickets;
	static std::unordered_map<PowerupType, Powerup *> powerups;
};
