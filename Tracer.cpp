#include "Tracer.h"
#include "Sphere.h"
#include "Math.h"

void Tracer::Trace(Image& image, Scene& scene, Camera& camera)
{
	for (int y = 0; y < image.height(); y++)
	{
		for (int x = 0; x < image.width(); x++)
		{
			glm::vec3 color{ 0, 0, 0 };
			for (int s = 0; s < m_samples; s++)
			{
				glm::vec2 viewport = camera.ScreenToViewport({ x + random01(), y + random01() });
				viewport.y = 1 - (viewport.y);
				ray r = camera.ViewportToRay(viewport);

				raycastHit hit;
				color += scene.Trace(r, 0.001f, FLT_MAX, hit, m_depth);
			}
			color /= (float)m_samples;
			image.SetPoint({ x, y }, color);
		}
	}
}
