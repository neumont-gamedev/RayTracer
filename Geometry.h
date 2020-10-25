#pragma once
#include "Ray.h"

class Geometry
{
public:
	virtual bool Hit(const ray& r) = 0;
};

