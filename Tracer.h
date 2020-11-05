#pragma once
#include "Types.h"
#include "Image.h"
#include "Scene.h"
#include "Camera.h"

class Tracer
{
public:
	Tracer(int samples, int depth) : m_samples{ samples }, m_depth{ depth } {}

	void Trace(Image& image, Scene& scene, Camera& camera);

private:
	int m_samples{ 1 };
	int m_depth{ 5 };
};