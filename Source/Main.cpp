#include <ctime>
#include <iostream>
#include "BrickBreaker/BrickBreaker.h"

#include <Core/Engine.h>

using namespace std;

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	// Create a window property structure
	WindowProperties wp;
	wp.name = "BrickBreaker";
	wp.resolution = glm::ivec2(580, 720);

	// Init the Engine and create a new window with the defined properties
	WindowObject *window = Engine::Init(wp);

	// Create a new 2D world and start running it
	World *world = new BrickBreaker();
	world->Init();
	world->Run();

	// Signals to the Engine to release the OpenGL context
	Engine::Exit();

	return 0;
}
