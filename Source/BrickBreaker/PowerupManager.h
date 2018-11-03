#pragma once

#include "PowerupTicket.h"
#include "Powerups.h"
#include <vector>
#include <unordered_map>

/**
 * Singleton class for managing the powerups and the tickets from which they are born.
 */
class PowerupManager
{
public:
	static PowerupManager *instance; // singleton instance
	std::unordered_map<Mesh *, glm::mat3> activePowerupsMeshes;

	static PowerupManager *GetInstance();
	~PowerupManager();
	void SetViewportSize(glm::vec2 size);

	void MaySpawn(float x, float y); // allow powerup ticket spawn at the given position
	void UpdatePowerups(float deltaTime);
	void KillPowerupTicket(PowerupTicket *powerupTicket);
	std::vector<PowerupTicket *> &GetTickets();
	void EnablePowerup(PowerupTicket *powerupTicket);
	void KillAllTickets();
	bool IsPowerupActive(PowerupType type);
	Powerup *GetPowerup(PowerupType type);

	friend class BottomWall;

private:
	PowerupManager();
	glm::vec2 viewportSize;
	std::vector<PowerupTicket *> powerupTickets;
	std::unordered_map<PowerupType, Powerup *> powerups;
};
