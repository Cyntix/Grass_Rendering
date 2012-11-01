/****************************************************************************
|*  Vector2.h
|*
|*  Definition of a 2d double vector and its basic functionality.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Balint Miklos, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _VECTOR2_H
#define _VECTOR2_H


#include <math.h>
#include <assert.h>


class Vector2 {
public:
	// Vector2 Methods
	Vector2(double _x=0, double _y=0)
		: x(_x), y(_y) {
	}

	Vector2 operator+(const Vector2 &v) const {
		return Vector2(x + v.x, y + v.y);
	}
	
	Vector2& operator+=(const Vector2 &v) {
		x += v.x; y += v.y;
		return *this;
	}

	Vector2 operator-(const Vector2 &v) const {
		return Vector2(x - v.x, y - v.y);
	}
	
	Vector2& operator-=(const Vector2 &v) {
		x -= v.x; y -= v.y;
		return *this;
	}

	bool operator==(const Vector2 &v) const {
		return x == v.x && y == v.y;
	}

	Vector2 operator*(double f) const {
		return Vector2(f*x, f*y);
	}
	
	Vector2 &operator*=(double f) {
		x *= f; y *= f;
		return *this;
	}

	Vector2 operator/(double f) const {
		assert(f!=0);
		double inv = 1.f / f;
		return Vector2(x * inv, y * inv);
	}
	
	Vector2 &operator/=(double f) {
		assert(f!=0);
		double inv = 1.f / f;
		x *= inv; y *= inv;
		return *this;
	}

	Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	double &operator[](int i) {
		assert(i >= 0 && i <= 1);
		switch (i) {
			case 0: return x;
			case 1: return y;
		}
	} 

	Vector2 normalize() { 
		assert(length() > 0);
		double l = length();
		x /= l;
		y /= l;
		return *this;
	}

	Vector2 clamp01() { 
		if (x>1.f) x=1.f;
			else if (x<0.f) x=0.f;
		if (y>1.f) y=1.f;
			else if (y<0.f) y=0.f;
		return *this;
	}


	double lengthSquared() const { return x*x + y*y; }
	
	double length() const { return sqrt(lengthSquared()); }

	// Vector2 Data
	double x, y;
};


#endif //_VECTOR2_H
