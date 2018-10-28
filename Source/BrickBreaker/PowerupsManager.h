#pragma once

#include "Powerup.h"
#include <vector>

class PowerupsManager
{
public:
	PowerupsManager();
	~PowerupsManager();
	void MaySpawn(float x, float y);
	void Update(float deltaTime);
	void EndPowerup(Powerup* p);
	std::vector<Powerup *> &GetPowerups();

private:
	 std::vector<Powerup *> powerup;
};
