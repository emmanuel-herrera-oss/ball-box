#ifndef __VEC2D_HPP__
#define __VEC2D_HPP__

class Vec2d {
public:
	Vec2d();
	Vec2d(float x, float y);
	Vec2d add(const Vec2d& otherVector) const;
	Vec2d subtract(const Vec2d& otherVector) const;
	Vec2d scalar_multiply(float scalar) const;
	float magnitude() const;
	Vec2d normalize() const;
	float get_x() const;
	float get_y() const;
	Vec2d get_orthogonal() const;
	float dot(const Vec2d& otherVector) const;
private:
	float x;
	float y;
};

#endif