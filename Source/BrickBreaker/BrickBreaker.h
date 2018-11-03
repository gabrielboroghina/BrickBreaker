#pragma once

#include "Component/SimpleScene.h"
#include "Ball.h"
#include "Paddle.h"
#include "Walls.h"
#include "Bricks.h"
#include "PowerupManager.h"

using namespace Object;

class BrickBreaker : public SimpleScene
{
public:
	BrickBreaker();
	virtual ~BrickBreaker();

	void Init() override;

private:
	const int bricksLines = 10, bricksCols = 10;

	Object::Ball *ball;
	Object::Paddle *paddle;
	Object::Walls *walls;
	Object::Bricks *bricks;
	Mesh *live[3];
	PowerupManager *powerupsManager;

	bool isGameOver;
	int lives; // remaining lives
	glm::vec2 viewportSize, windowScale;

	void CheckCollisions();
	void ResetWithLiveLost();
	void ResetGame();
	void RenderLives();
	void RenderWalls();
	void HandleBrickCollisions(float xBall, float yBall);
	void HandlePowerupsCollisions(float xBall, float yBall);

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
};
