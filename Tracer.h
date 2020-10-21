#pragma once
#include "Types.h"
#include "Image.h"

class Tracer
{
public:
	Tracer() {}

	void Trace(Image& image);

private:
	glm::vec3 m_origin{ 0, 0, 0 };
};