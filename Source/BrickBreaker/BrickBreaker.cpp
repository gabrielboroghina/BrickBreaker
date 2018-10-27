#include "BrickBreaker.h"

#include <Core/Engine.h>
#include "Transform2D.h"
#include "BasicMeshes.h"

using namespace std;

BrickBreaker::BrickBreaker() {}

BrickBreaker::~BrickBreaker() {}

void BrickBreaker::Init()
{
	glm::vec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// initialize objects
	lives = 3;
	for (int i = 0; i < lives; i++)
		live[i] = CreateHeart();

	ball = new Object2D::Ball(resolution.x / 2.0f, 45.0f);
	paddle = new Object2D::Paddle();
	walls = new Walls(resolution.x, resolution.y);
	bricks = new Bricks(10, 10, resolution.y, resolution.x);
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
		for (int j = 0; j < bricks->numBrickCols; j++)
			if (bricks->bricks.count(i * bricks->numBrickCols + j)) {

				// compute brick iterator in bricks vector
				std::tie(xMin, xMax, yMin, yMax) = bricks->GetBrickBounds(i, j);

				// vertical collision
				if (xBall + collisionDist >= xMin && xBall - collisionDist <= xMax) {
					if ((yBall >= yMax && yBall - yMax <= collisionDist && ball->vy < 0) || // collision from top
						(yBall <= yMin && yMin - yBall <= collisionDist && ball->vy > 0)) {
						// collision from bottom
						ball->ReflectY();
						bricks->bricks.erase(i * bricks->numBrickCols + j);
					}
				}

				// horizontal collision
				if (yBall + collisionDist >= yMin && yBall - collisionDist <= yMax) {
					if ((xBall >= xMax && xBall - xMax <= collisionDist && ball->vx < 0) || // collision from left
						(xBall <= xMin && xMin - xBall <= collisionDist && ball->vx > 0)) {
						// collision from right
						ball->ReflectX();
						bricks->bricks.erase(i * bricks->numBrickCols + j);
					}
				}
			}
}

void BrickBreaker::CheckCollisions()
{
	float xBall = ball->transformMatrix[2][0];
	float yBall = ball->transformMatrix[2][1];
	glm::ivec2 resolution = window->GetResolution();

	// check if ball exited the game area
	if (xBall < 0 || yBall < 0 || xBall > resolution.x || yBall > resolution.y)
		ResetWithLiveLost();

	// collision with the paddle
	if (xBall >= paddle->xCenter - paddle->length / 2 && xBall <= paddle->xCenter + paddle->length / 2 &&
		yBall - paddle->yTop <= collisionDist && ball->vy <= 0)
		ball->ReflectAngled((xBall - paddle->xCenter) / (paddle->length / 2));

	// collision with a wall
	float yTopBound = resolution.y - walls->wallWidth;
	float xLeftBound = walls->wallWidth;
	float xRightBound = resolution.x - walls->wallWidth;

	if ((xBall - xLeftBound <= collisionDist || xRightBound - xBall <= collisionDist) && ball->vy > 0 &&
		resolution.y - walls->verticalHeight - yBall <= collisionDist && yBall < resolution.y - walls->verticalHeight)
		ball->ReflectY();

	if (yTopBound - yBall <= collisionDist && ball->vy > 0)
		ball->ReflectY();

	if (((xBall - xLeftBound <= collisionDist && ball->vx < 0) ||
			(xRightBound - xBall <= collisionDist && ball->vx > 0)) &&
		resolution.y - walls->verticalHeight <= yBall + collisionDist)
		ball->ReflectX();

	// collision with a brick
	HandleBrickCollisions(xBall, yBall);
}

void BrickBreaker::ResetWithLiveLost()
{
	lives--;
	ball->Attach();

	if (!lives) {
		// GAME OVER
	}
}

void BrickBreaker::RenderLives()
{
	float livePos[3][2] = {
		{20, 15},
		{40, 15},
		{60, 15}
	};

	for (int i = 0; i < lives; i++)
		RenderMesh2D(live[i], shaders["VertexColor"], Transform2D::Translate(livePos[i][0], livePos[i][1]));
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	glm::vec2 resolution = window->GetResolution();

	if (!ball->isAttached)
		CheckCollisions();

	// render walls
	RenderMesh2D(walls->meshLeft, shaders["VertexColor"],
	             Transform2D::Translate(walls->wallWidth / 2, resolution.y - walls->verticalHeight / 2));
	RenderMesh2D(walls->meshRight, shaders["VertexColor"],
	             Transform2D::Translate(resolution.x - walls->wallWidth / 2, resolution.y - walls->verticalHeight / 2));
	RenderMesh2D(walls->meshTop, shaders["VertexColor"],
	             Transform2D::Translate(resolution.x / 2.0f, resolution.y - walls->wallWidth / 2));

	float cursorXPos = (float)window->GetCursorPosition().x;
	paddle->CenterAtXPos(cursorXPos);
	RenderMesh2D(paddle->mesh, shaders["VertexColor"], Transform2D::Translate(paddle->xCenter, paddle->yCenter));

	ball->Update(deltaTimeSeconds, cursorXPos, 45.0f);
	RenderMesh2D(ball->mesh, shaders["VertexColor"], ball->transformMatrix);

	RenderLives();

	// render bricks
	for (auto pp : bricks->bricks)
		RenderMesh2D(pp.second, shaders["VertexColor"], bricks->translateMatrix);
}

void BrickBreaker::FrameEnd() {}

void BrickBreaker::OnInputUpdate(float deltaTime, int mods) {}

void BrickBreaker::OnKeyPress(int key, int mods) { }

void BrickBreaker::OnKeyRelease(int key, int mods) {}

void BrickBreaker::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void BrickBreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	switch (button) {
		case GLFW_MOUSE_BUTTON_2:
			if (ball->isAttached) ball->Detach();
			break;
	}
}

void BrickBreaker::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void BrickBreaker::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void BrickBreaker::OnWindowResize(int width, int height) {}
