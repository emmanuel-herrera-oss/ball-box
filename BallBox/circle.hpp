#ifndef __CIRCLE_HPP__
#define __CIRCLE_HPP__

#include "color.hpp"
#include "vec2d.hpp"

class Circle {
public:
	int id;
	float radius;
	Color color;
	Vec2d position;
	Vec2d velocity;
};

#endif