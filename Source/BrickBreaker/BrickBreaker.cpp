#include "BrickBreaker.h"

#include <Core/Engine.h>
#include "Transform2D.h"
#include "BasicMeshes.h"

BrickBreaker::BrickBreaker() : isGameOver(false), lives(3), windowScale(glm::vec2(1, 1)) {}

BrickBreaker::~BrickBreaker()
{
	delete bricks;
	delete walls;
	delete paddle;
	delete ball;
	delete powerupsManager;
	for (int i = 0; i < 3; i++) delete live[i];
}

void BrickBreaker::Init()
{
	glm::vec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	viewportSize = resolution;

	// initialize objects
	for (int i = 0; i < lives; i++)
		live[i] = CreateHeart();

	ball = new Object2D::Ball(viewportSize.x / 2.0f, 45.0f);
	paddle = new Object2D::Paddle();
	walls = new Walls(viewportSize.x, viewportSize.y);
	bricks = new Bricks(10, 10, viewportSize);

	powerupsManager = PowerupManager::GetInstance();
	PowerupManager::SetViewportSize(viewportSize);
}

void BrickBreaker::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void BrickBreaker::HandleBrickCollisions(float xBall, float yBall)
{
	float xMin, xMax, yMin, yMax;

	for (int i = 0; i < bricks->numBrickLines; i++)
		for (int j = 0; j < bricks->numBrickCols; j++) {
			int brickIndex = i * bricks->numBrickCols + j;
			if (bricks->brick.count(brickIndex) && !bricks->WasBlasted(brickIndex)) {

				// compute brick iterator in bricks vector
				std::tie(xMin, xMax, yMin, yMax) = bricks->GetBrickBounds(i, j);

				bool collisionDetected = false;
				// vertical collision
				if (xBall + collisionDist >= xMin && xBall - collisionDist <= xMax) {
					if ((yBall >= yMax && yBall - yMax <= collisionDist && ball->vy < 0) || // collision from top
						(yBall <= yMin && yMin - yBall <= collisionDist && ball->vy > 0)) {
						// collision from bottom
						ball->ReflectY();
						collisionDetected = true;
					}
				}

				// horizontal collision
				if (yBall + collisionDist >= yMin && yBall - collisionDist <= yMax) {
					if ((xBall >= xMax && xBall - xMax <= collisionDist && ball->vx < 0) || // collision from left
						(xBall <= xMin && xMin - xBall <= collisionDist && ball->vx > 0)) {
						// collision from right
						ball->ReflectX();
						collisionDetected = true;
					}
				}

				// bullets (shooter)
				Shooter *shooter = static_cast<Shooter *>(PowerupManager::GetPowerup(SHOOTER));
				if (shooter)
					for (auto it = shooter->GetBulletMeshes().begin(); it != shooter->GetBulletMeshes().end();) {
						glm::vec3 bulletPos = it->second * glm::vec3(0, 0, 1);
						if (bulletPos.x >= xMin && bulletPos.x <= xMax && bulletPos.y >= yMin && bulletPos.y <= yMax) {
							bricks->Blast(brickIndex); // don't spawn from bullet-destroyed bricks
							auto auxIt = it;
							++it;
							shooter->KillBullet(auxIt->first);
						}
						else ++it;
					}

				if (collisionDetected) {
					bricks->Blast(brickIndex);
					powerupsManager->MaySpawn((xMin + xMax) / 2, (yMin + yMax) / 2);
				}
			}
		}
}

void BrickBreaker::HandlePowerupsCollisions(float xBall, float yBall)
{
	float xMin = paddle->xCenter - paddle->length;
	float xMax = paddle->xCenter + paddle->length;

	// check powerup tickets collisions
	for (auto &powerupTicket : powerupsManager->GetTickets()) {
		for (auto corner : powerupTicket->GetCorners())
			if (corner.x >= xMin && corner.x <= xMax &&
				corner.y <= paddle->yTop) {
				// detected collision; activate powerup and delete the mesh
				if (powerupTicket->type == SHOOTER) {
					ball->Attach();
					ball->Scale(0);
				}
				powerupsManager->EnablePowerup(powerupTicket);
				break;
			}
	}

	// check active powerups

	// bottom wall
	if (powerupsManager->IsPowerupActive(BOTTOM_WALL) && yBall - BottomWall::yTop <= collisionDist)
		ball->ReflectY();
}

void BrickBreaker::CheckCollisions()
{
	float xBall = ball->transformMatrix[2][0];
	float yBall = ball->transformMatrix[2][1];

	// check if ball exited the game area
	if ((xBall < 0 || yBall < 0 || xBall > viewportSize.x || yBall > viewportSize.y) && !ball->isAttached)
		ResetWithLiveLost();

	// collision with the paddle
	if (xBall >= paddle->xCenter - paddle->length / 2 && xBall <= paddle->xCenter + paddle->length / 2 &&
		yBall - paddle->yTop <= collisionDist && ball->vy <= 0)
		ball->ReflectAngled((xBall - paddle->xCenter) / (paddle->length / 2));

	// collision with a wall
	float yTopBound = viewportSize.y - walls->wallWidth;
	float xLeftBound = walls->wallWidth;
	float xRightBound = viewportSize.x - walls->wallWidth;

	if ((xBall - xLeftBound <= collisionDist || xRightBound - xBall <= collisionDist) && ball->vy > 0 &&
		viewportSize.y - walls->verticalHeight - yBall <= collisionDist && yBall < viewportSize.y - walls->
		verticalHeight)
		ball->ReflectY();

	if (yTopBound - yBall <= collisionDist && ball->vy > 0)
		ball->ReflectY();

	if (((xBall - xLeftBound <= collisionDist && ball->vx < 0) ||
			(xRightBound - xBall <= collisionDist && ball->vx > 0)) &&
		viewportSize.y - walls->verticalHeight <= yBall + collisionDist)
		ball->ReflectX();

	// collision with a brick
	HandleBrickCollisions(xBall, yBall);

	// collision between a powerup ticket and the paddle or between the ball and a powerup ticket
	HandlePowerupsCollisions(xBall, yBall);
}

void BrickBreaker::ResetWithLiveLost()
{
	lives--;
	ball->Attach();

	if (!lives) {
		// GAME OVER
		ball->Scale(0);
		isGameOver = true;
	}
}

void BrickBreaker::ResetGame()
{
	isGameOver = false;
	lives = 3;
	ball->Scale(1);
	bricks = new Bricks(10, 10, viewportSize);
}

void BrickBreaker::RenderLives()
{
	float livePos[3][2] = {
		{25, 20},
		{45, 20},
		{65, 20}
	};

	for (int i = 0; i < lives; i++)
		RenderMesh2D(live[i], shaders["VertexColor"], Transform2D::Translate(livePos[i][0], livePos[i][1]));
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	if (isGameOver)
		return;

	CheckCollisions();

	Shader *colorShader = shaders["VertexColor"];
	// render walls
	using Transform2D::Translate;
	RenderMesh2D(walls->meshLeft, colorShader,
	             Translate(walls->wallWidth / 2, viewportSize.y - walls->verticalHeight / 2));

	RenderMesh2D(walls->meshRight, colorShader, Translate(walls->topWallWidth - walls->wallWidth / 2,
	                                                      viewportSize.y - walls->verticalHeight / 2));

	RenderMesh2D(walls->meshTop, colorShader,
	             Translate(walls->topWallWidth / 2.0f, viewportSize.y - walls->wallWidth / 2));

	float cursorXPos = window->GetCursorPosition().x / windowScale.x;
	paddle->CenterAtXPos(cursorXPos);
	RenderMesh2D(paddle->mesh, colorShader, Translate(paddle->xCenter, paddle->yCenter));

	ball->Update(deltaTimeSeconds, cursorXPos, 45.0f);
	RenderMesh2D(ball->mesh, colorShader, ball->transformMatrix);

	RenderLives();

	// render bricks
	bricks->Update(deltaTimeSeconds);
	for (auto &pp : bricks->brick)
		RenderMesh2D(pp.second, colorShader, bricks->GetTransformMatrix(pp.first));

	// render powerups
	powerupsManager->UpdatePowerups(deltaTimeSeconds);
	for (auto &powerupTicket : powerupsManager->GetTickets())
		RenderMesh2D(powerupTicket->mesh, colorShader, powerupTicket->GetTransformMatrix());

	for (auto &activePowerup : PowerupManager::activePowerupsMeshes)
		RenderMesh2D(activePowerup.first, colorShader, activePowerup.second);

	Shooter *shooter = static_cast<Shooter *>(PowerupManager::GetPowerup(SHOOTER));
	if (shooter) {
		for (auto &bullet : shooter->GetBulletMeshes())
			RenderMesh2D(bullet.first, colorShader, bullet.second);

		if (shooter->HasEnded()) // reactivate the ball
			ball->Scale(1);
	}
}

void BrickBreaker::FrameEnd() {}

void BrickBreaker::OnInputUpdate(float deltaTime, int mods) {}

void BrickBreaker::OnKeyPress(int key, int mods) { }

void BrickBreaker::OnKeyRelease(int key, int mods) {}

void BrickBreaker::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void BrickBreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	switch (button) {
		case GLFW_MOUSE_BUTTON_2: // left button
			if (isGameOver) ResetGame();

			if (powerupsManager->IsPowerupActive(SHOOTER)) {
				static_cast<Shooter *>(powerupsManager->GetPowerup(SHOOTER))->Fire(
					(float)window->GetCursorPosition().x / windowScale.x);
			}
			else {
				if (ball->isAttached) ball->Detach();
			}
			break;
	}
}

void BrickBreaker::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void BrickBreaker::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void BrickBreaker::OnWindowResize(int width, int height)
{
	windowScale.x = width / viewportSize.x;
	windowScale.y = height / viewportSize.y;
}
