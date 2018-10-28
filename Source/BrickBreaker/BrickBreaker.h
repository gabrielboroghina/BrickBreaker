#pragma once

#include "Component/SimpleScene.h"
#include "Ball.h"
#include "Paddle.h"
#include "Walls.h"
#include "Bricks.h"

class BrickBreaker : public SimpleScene
{
public:
	BrickBreaker();
	virtual ~BrickBreaker();

	void Init() override;

private:
	bool isGameOver;
	Object2D::Ball *ball;
	Object2D::Paddle *paddle;
	Walls *walls;
	Mesh *live[3];
	Bricks *bricks;

	int lives;
	const float collisionDist = 8;
	float windowWidthScale, windowHeightScale;
	glm::vec2 viewportSize;

	void CheckCollisions();
	void ResetWithLiveLost();
	void RenderLives();
	void RenderBricks();
	void HandleBrickCollisions(float xBall, float yBall);

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
