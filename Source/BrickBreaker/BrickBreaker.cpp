#include "BrickBreaker.h"

#include <Core/Engine.h>
#include "Transform2D.h"

using namespace std;

BrickBreaker::BrickBreaker() {}

BrickBreaker::~BrickBreaker() {}

void BrickBreaker::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	lives = 3;
	ball = new Object2D::Ball(resolution.x / 2.0f, 45.0f);
	paddle = new Object2D::Paddle();
	walls = new Walls(resolution.x, resolution.y);
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

void BrickBreaker::CheckCollisions()
{
	float xBall = ball->transformMatrix[2][0];
	float yBall = ball->transformMatrix[2][1];

	// check if ball exited the game area
	if (xBall < 0 || yBall < 0 || xBall > window->GetResolution().x || yBall > window->GetResolution().y)
		ResetWithLiveLost();

	// collision with the paddle
	if (xBall >= paddle->xCenter - paddle->length / 2 && xBall <= paddle->xCenter + paddle->length / 2 &&
		yBall - paddle->yTop <= collisionDist && ball->vy < 0)
		ball->ReflectAngled((xBall - paddle->xCenter) / (paddle->length / 2));

	// collision with a wall
	float yTopBound = window->GetResolution().y - walls->wallWidth;
	float xLeftBound = walls->wallWidth;
	float xRightBound = window->GetResolution().x - walls->wallWidth;

	if (yTopBound - yBall <= collisionDist && ball->vy > 0)
		ball->ReflectY();
	else if ((xBall - xLeftBound <= collisionDist && ball->vx < 0) ||
		(xRightBound - xBall <= collisionDist && ball->vx > 0))
		ball->ReflectX();
}

void BrickBreaker::ResetWithLiveLost()
{
	lives--;
	ball->Attach();
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();

	CheckCollisions();

	// render walls
	RenderMesh2D(walls->meshLeft, shaders["VertexColor"],
	             Transform2D::Translate(walls->wallWidth / 2, resolution.y - walls->verticalHeight / 2));
	RenderMesh2D(walls->meshRight, shaders["VertexColor"],
	             Transform2D::Translate(resolution.x - walls->wallWidth / 2, resolution.y - walls->verticalHeight / 2));
	RenderMesh2D(walls->meshTop, shaders["VertexColor"],
	             Transform2D::Translate(resolution.x / 2.0, resolution.y - walls->wallWidth / 2));

	paddle->CenterAtXPos(window->GetCursorPosition().x);
	RenderMesh2D(paddle->mesh, shaders["VertexColor"], Transform2D::Translate(paddle->xCenter, paddle->yCenter));

	ball->Update(deltaTimeSeconds, window->GetCursorPosition().x, 45.0f);
	RenderMesh2D(ball->mesh, shaders["VertexColor"], ball->transformMatrix);
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
