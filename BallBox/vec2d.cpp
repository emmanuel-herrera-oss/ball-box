#include "vec2d.hpp"
#include "math.h"

Vec2d::Vec2d() {
	this->x = 0;
	this->y = 0;
}
Vec2d::Vec2d(float x, float y) {
	this->x = x;
	this->y = y;
}

Vec2d Vec2d::add(const Vec2d& otherVector) const {
	return Vec2d(this->x + otherVector.x, this->y + otherVector.y);
}

Vec2d Vec2d::subtract(const Vec2d& otherVector) const {
	return Vec2d(this->x - otherVector.x, this->y - otherVector.y);
}

Vec2d Vec2d::scalar_multiply(float scalar) const {
	return Vec2d(this->x * scalar, this->y * scalar);
}

float Vec2d::magnitude() const {
	return sqrtf(this->x * this->x + this->y * this->y);
}

Vec2d Vec2d::normalize() const {
	float magnitude = this->magnitude();
	return Vec2d(this->x / magnitude, this->y / magnitude);
}

float Vec2d::get_x() const
{
	return this->x;
}

float Vec2d::get_y() const
{
	return this->y;
}

Vec2d Vec2d::get_orthogonal() const
{
	return Vec2d(-this->y, this->x);
}

float Vec2d::dot(const Vec2d& otherVector) const
{
	return this->x * otherVector.x + this->y * otherVector.y;
}