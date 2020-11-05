#include "Canvas.h"
#include "Image.h"
#include "Tracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"
#include "Material.h"
#include "Camera.h"
#include "SDL.h"
#include <iostream>
#include <vector>

int main(int, char**)
{
	const int width = 800;
	const int height = 600;
	const int samples = 20;
	const int depth = 20;

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
	Image image{ width, height };
	Camera camera{ glm::vec3{2, 2, 5}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0}, 80.0f, &image };
	Tracer tracer{ samples, depth };
	Scene scene;

	scene.Add(new Sphere{ {2, 2, -4}, 1, new Lambertian{ glm::vec3{0, 1, 0} } });
	scene.Add(new Sphere{ {0, 0, -6}, 2, new Metal{ glm::vec3{1, 0, 1}, 0.1f } });
	scene.Add(new Sphere{ {-2, -2, -3}, 1, new Lambertian{ glm::vec3{1, 1, 1} } });
	scene.Add(new Sphere{ {0, 0, 0}, 1, new Dielectric{ glm::vec3{0.8f, 0.8f, 1}, 1.33f } });
	scene.Add(new Plane{ {0, -2, 0}, {0, 1, 0}, new Metal{ glm::vec3{0.5f, 0.5f, 0.5f}, 1.0f } });
	image.Clear({1, 1, 0});
	tracer.Trace(image, scene, camera);

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
		canvas.DrawImage(image);
		canvas.Update();

		SDL_RenderClear(renderer);
		canvas.Draw(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}
