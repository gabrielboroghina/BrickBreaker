#include "Powerup.h"

#include "BasicMeshes.h"
#include "Transform2D.h"
#include <unordered_map>

std::unordered_map<PowerupType, glm::vec3> colorByType = {
	{PowerupType::BOTTOM_WALL, glm::vec3(1, 1, 0)}
};

Powerup::Powerup(PowerupType type, float x, float y)
{
	translateMatrix = Transform2D::Translate(x, y);
	rotateMatrix = glm::mat3(1);

	mesh = CreateRect("powerup", glm::vec3(0, 0, 1), 15, 15, colorByType[type], true);
	velocity = (float)(rand() % 3 + 1) * 100;
}

Powerup::~Powerup()
{
	delete mesh;
}

void Powerup::Update(float deltaTime)
{
	translateMatrix *= Transform2D::Translate(0, -velocity * deltaTime);
	rotateMatrix *= Transform2D::Rotate(deltaTime * 2);

	if ((translateMatrix * rotateMatrix * glm::vec3(0, 0, 1)).y <= 0)
		delete mesh, mesh = nullptr;
}

glm::mat3x3 Powerup::GetTransformMatrix()
{
	return translateMatrix * rotateMatrix;
}
