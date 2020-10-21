#include "Tracer.h"

void Tracer::Trace(Image& image)
{
	for (int y = 0; y < image.height(); y++)
	{
		for (int x = 0; x < image.width(); x++)
		{ 
			glm::vec3 p;
			p.x = x / (float)image.width();
			p.y = 1 - (y / (float)image.height());
			p.z = 0;

			glm::vec3 direction = glm::vec3{ glm::vec2{p} * 2.0f - 1.0f, 1};
			direction = glm::normalize(direction);

			image.SetPoint({ x, y }, direction);


		}
	}
}
