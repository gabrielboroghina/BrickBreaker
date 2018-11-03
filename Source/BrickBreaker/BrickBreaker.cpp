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

	// initialize the 2D objects
	for (int i = 0; i < lives; i++)
		live[i] = CreateHeart();

	paddle = new Object::Paddle();
	ball = new Object::Ball(viewportSize.x / 2.0f, paddle->yTop);
	walls = new Object::Walls(viewportSize);
	bricks = new Object::Bricks(bricksLines, bricksCols, viewportSize);

	powerupsManager = PowerupManager::GetInstance();
	powerupsManager->SetViewportSize(viewportSize);
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
			// compute brick index from brick position
			int brickIndex = i * bricks->numBrickCols + j;
			if (bricks->brick.count(brickIndex) && !bricks->WasBlasted(brickIndex)) {

				bool collisionDetected = false;
				std::tie(xMin, xMax, yMin, yMax) = bricks->GetBrickBounds(i, j);

				// vertical collision
				if (xBall + ball->radius >= xMin && xBall - ball->radius <= xMax) {
					if ((yBall >= yMax && yBall - yMax <= ball->radius && ball->vy < 0) || // collision from top
						(yBall <= yMin && yMin - yBall <= ball->radius && ball->vy > 0)) // collision from bottom
					{
						ball->ReflectY();
						collisionDetected = true;
					}
				}

				// horizontal collision
				if (yBall + ball->radius >= yMin && yBall - ball->radius <= yMax) {
					if ((xBall >= xMax && xBall - xMax <= ball->radius && ball->vx < 0) || // collision from left
						(xBall <= xMin && xMin - xBall <= ball->radius && ball->vx > 0)) // collision from right
					{
						ball->ReflectX();
						collisionDetected = true;
					}
				}

				// bullet - brick collision (shooter powerup)
				Shooter *shooter = static_cast<Shooter *>(powerupsManager->GetPowerup(SHOOTER));
				if (shooter)
					for (auto it = shooter->GetBulletMeshes().begin(); it != shooter->GetBulletMeshes().end();) {
						glm::vec3 bulletPos = it->second * glm::vec3(0, paddle->yTop, 1); // bullet's center
						if (bulletPos.x >= xMin && bulletPos.x <= xMax && bulletPos.y >= yMin && bulletPos.y <= yMax) {
							bricks->Blast(brickIndex); // don't spawn from bullet-destroyed bricks
							auto auxIt = it;
							++it;
							shooter->KillBullet(auxIt->first);
							break;
						}
						else ++it;
					}

				if (collisionDetected) {
					bricks->Blast(brickIndex);
					powerupsManager->MaySpawn((xMin + xMax) / 2, (yMin + yMax) / 2); // allow powerup spawning
				}
			}
		}
}

void BrickBreaker::HandlePowerupsCollisions(float xBall, float yBall)
{
	// compute paddle bounds
	float xMinPaddle = paddle->xCenter - paddle->length / 2.0f;
	float xMaxPaddle = paddle->xCenter + paddle->length / 2.0f;

	// check powerup tickets collisions
	for (auto &powerupTicket : powerupsManager->GetTickets()) {
		for (auto corner : powerupTicket->GetCorners())
			if (corner.x >= xMinPaddle && corner.x <= xMaxPaddle && corner.y <= paddle->yTop) {
				// detected collision; activate powerup and delete the ticket
				if (powerupTicket->type == SHOOTER) {
					// disable the ball
					ball->Attach();
					ball->Scale(0, 0);
				}
				else if (powerupTicket->type == FAT_BALL && !powerupsManager->IsPowerupActive(SHOOTER)) {
					ball->Resize(2);
				}
				powerupsManager->EnablePowerup(powerupTicket);
				break;
			}
	}

	// check bottom wall powerup
	if (powerupsManager->IsPowerupActive(BOTTOM_WALL) &&
		yBall - BottomWall::yTop <= ball->radius && yBall >= BottomWall::yTop)
		ball->ReflectY();
}

void BrickBreaker::CheckCollisions()
{
	// compute coordinates of the ball's center
	float xBall = ball->translateMatrix[2][0];
	float yBall = ball->translateMatrix[2][1];

	// check if ball exited the game area
	if ((xBall < 0 || yBall < 0 || xBall > viewportSize.x || yBall > viewportSize.y) && !ball->isAttached)
		ResetWithLiveLost();

	// ball - paddle collisions
	if (xBall >= paddle->xCenter - paddle->length / 2 && xBall <= paddle->xCenter + paddle->length / 2 &&
		yBall - paddle->yTop <= ball->radius && yBall >= paddle->yCenter && ball->vy <= 0)
		ball->ReflectAngled((xBall - paddle->xCenter) / (paddle->length / 2));

	// ball - wall collisions
	float yTopBound = viewportSize.y - walls->thickness;
	float xLeftBound = walls->thickness;
	float xRightBound = viewportSize.x - walls->thickness;

	// * vertical collision with the left or right wall (on its bottom)
	if ((xBall - xLeftBound <= ball->radius || xRightBound - xBall <= ball->radius) && ball->vy > 0 &&
		viewportSize.y - walls->verticalHeight - yBall <= ball->radius &&
		yBall < viewportSize.y - walls->verticalHeight)
		ball->ReflectY();

	// * vertical collision with the top wall
	if (yTopBound - yBall <= ball->radius && ball->vy > 0)
		ball->ReflectY();

	// * horizontal collision with the left or right wall
	if (((xBall - xLeftBound <= ball->radius && ball->vx < 0) ||
			(xRightBound - xBall <= ball->radius && ball->vx > 0)) &&
		viewportSize.y - walls->verticalHeight <= yBall + ball->radius)
		ball->ReflectX();

	// ball - brick or bullet - brick collisions
	HandleBrickCollisions(xBall, yBall);

	// powerup ticket - paddle or ball - powerup ticket collisions
	HandlePowerupsCollisions(xBall, yBall);
}

void BrickBreaker::ResetWithLiveLost()
{
	lives--;
	ball->Attach();

	if (!lives) {
		// GAME OVER
		isGameOver = true;
		powerupsManager->KillAllTickets();
	}
}

void BrickBreaker::ResetGame()
{
	isGameOver = false;
	lives = 3;
	ball->Scale(1, 1);
	bricks = new Bricks(bricksLines, bricksCols, viewportSize);
}

void BrickBreaker::RenderLives()
{
	float livePos[3][2] = {
		{25, 18},
		{45, 18},
		{65, 18}
	};

	for (int i = 0; i < lives; i++)
		RenderMesh2D(live[i], shaders["VertexColor"], Transform2D::Translate(livePos[i][0], livePos[i][1]));
}

void BrickBreaker::RenderWalls()
{
	using Transform2D::Translate;
	Shader *colorShader = shaders["VertexColor"];

	RenderMesh2D(walls->meshLeft, colorShader,
	             Translate(walls->thickness / 2, viewportSize.y - walls->verticalHeight / 2));

	RenderMesh2D(walls->meshRight, colorShader,
	             Translate(walls->topWallWidth - walls->thickness / 2, viewportSize.y - walls->verticalHeight / 2));

	RenderMesh2D(walls->meshTop, colorShader,
	             Translate(walls->topWallWidth / 2.0f, viewportSize.y - walls->thickness / 2));
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	if (isGameOver) return;

	CheckCollisions();

	Shader *colorShader = shaders["VertexColor"];

	float cursorXPos = window->GetCursorPosition().x / windowScale.x;
	paddle->CenterAtXPos(cursorXPos);
	RenderMesh2D(paddle->mesh, colorShader, Transform2D::Translate(paddle->xCenter, paddle->yCenter));

	ball->Update(deltaTimeSeconds, cursorXPos, paddle->yTop);
	RenderMesh2D(ball->mesh, colorShader, ball->GetTransformMatrix());

	RenderWalls();
	RenderLives();

	// render bricks
	bricks->Update(deltaTimeSeconds);
	for (auto &pp : bricks->brick)
		RenderMesh2D(pp.second, colorShader, bricks->GetTransformMatrix(pp.first));

	// render powerups
	powerupsManager->UpdatePowerups(deltaTimeSeconds);
	for (auto &powerupTicket : powerupsManager->GetTickets())
		RenderMesh2D(powerupTicket->mesh, colorShader, powerupTicket->GetTransformMatrix());

	for (auto &activePowerup : powerupsManager->activePowerupsMeshes)
		RenderMesh2D(activePowerup.first, colorShader, activePowerup.second);

	Shooter *shooter = static_cast<Shooter *>(powerupsManager->GetPowerup(SHOOTER));
	if (shooter) {
		// render bullets
		for (auto &bullet : shooter->GetBulletMeshes())
			RenderMesh2D(bullet.first, colorShader, bullet.second);

		if (shooter->HasEnded())
			ball->Scale(1, 1); // reactivate the ball
	}

	FatBall *fatBall = static_cast<FatBall *>(powerupsManager->GetPowerup(FAT_BALL));
	if (fatBall && fatBall->HasEnded() && !powerupsManager->IsPowerupActive(SHOOTER))
		ball->Resize(1);
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
		case GLFW_MOUSE_BUTTON_2: // click left
			if (isGameOver) {
				ResetGame();
			}
			else if (powerupsManager->IsPowerupActive(SHOOTER)) {
				static_cast<Shooter *>(powerupsManager->GetPowerup(SHOOTER))->Fire(
					(float)window->GetCursorPosition().x / windowScale.x);
			}
			else if (ball->isAttached) {
				ball->Detach(); // launch the ball
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
