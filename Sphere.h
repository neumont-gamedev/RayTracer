#pragma once
#include "Types.h"

class Sphere
{
public:
	Sphere() {}
	Sphere(const glm::vec3& center, float radius) : 
		m_center(center), 
		m_radius(radius) {}

	bool Hit(const ray& r);

private:
	glm::vec3 m_center{ 0, 0, 0 };
	float m_radius = 0;
};