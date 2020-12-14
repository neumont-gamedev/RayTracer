#include "Canvas.h"
#include "Image.h"
#include "Tracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"
#include "Material.h"
#include "Camera.h"
#include "SDL.h"
#include "Math.h"
#include <iostream>
#include <vector>

int main(int, char**)
{
	srand(123);

	const int width = 800;
	const int height = 600;
	const int samples = 10;
	const int depth = 20;
	const int numObjects = 30;
	const int numFrames = 1;

	glm::vec3 eye{ 0, 2, 5 };
	glm::vec3 lookAt{ 0, 0, -1 };
	glm::vec3 translate{ 0, 0, -0.1f };

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("2D Renderer", 100, 100, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	Canvas canvas{ renderer, width, height };
	//Image image{ width, height };
	Tracer tracer{ samples, depth };

	// create scene
	Scene scene;
	for (int i = 0; i < numObjects; i++)
	{
		glm::vec3 color = glm::rgbColor(glm::vec3{ random(0, 360), 1, 1 });
		glm::vec3 position = glm::vec3{ random(-20, 20), random(0, 10), random(-20, 0) };
		float radius = random(0.5f, 2.0f);

		Material* material = nullptr;
		switch (rand() % 3)
		{
		case 0:
			material = new Lambertian{ color };
			break;
		case 1:
			material = new Metal{ color, random(0, 0.3f) };
			break;
		case 2:
			material = new Dielectric{ color, random(1, 2.4f) };
			break;
		}

		scene.Add(new Sphere{ position, radius, material });
	}

	scene.Add(new Plane{ {0, -2, 0}, {0, 1, 0}, new Metal{ glm::vec3{0.5f, 0.5f, 0.5f}, 1.0f } });

	// render images
	std::vector<Image*> images;
	for (int i = 0; i < numFrames; i++)
	{
		Image* image = new Image{ width, height };
		Camera camera{ eye, eye + lookAt, glm::vec3{0, 1, 0}, 80.0f, image };

		tracer.Trace(*image, scene, camera);
		images.push_back(image);

		eye = eye + translate;

		std::cout << "frame: " << i << std::endl;
	}

	int frame = 0;
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		}

		canvas.Clear({ 0, 0, 0});

		canvas.DrawImage(*images[frame]);
		frame++;
		if (frame >= numFrames) frame = 0;

		canvas.Update();

		SDL_RenderClear(renderer);
		canvas.Draw(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}
