#pragma once

#include "Powerup.h"
#include <vector>
#include <unordered_map>

class PowerupsManager
{
public:
	float bottomWallYTop = 15;
	std::unordered_map<Mesh *, bool> activePowerupsMeshes;

	PowerupsManager(float viewportX);
	~PowerupsManager();
	void MaySpawn(float x, float y);
	void Update(float deltaTime);
	void EndPowerup(Powerup *powerup);
	std::vector<Powerup *> &GetPowerups();
	void EnablePowerup(Powerup *powerup);
	bool isPowerupActive(PowerupType type);

private:
	Mesh *bottomWall;
	float bottomWallTTL;
	const float bottomWallY = 10;

	std::vector<Powerup *> powerup;
};
