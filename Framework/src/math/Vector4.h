/****************************************************************************
|*  Vector4.h
|*
|*  Definition of a 4d double vector and its basic functionality.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Balint Miklos, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _VECTOR4_H
#define _VECTOR4_H


#include <math.h>
#include <assert.h>


class Vector4 {
public:
	// Vector4 Methods
	Vector4(double _x=0, double _y=0, double _z=0, double _w=0)
		: x(_x), y(_y), z(_z), w(_w) {
	}

	Vector4 operator+(const Vector4 &v) const {
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	
	Vector4& operator+=(const Vector4 &v) {
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Vector4 operator-(const Vector4 &v) const {
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	
	Vector4& operator-=(const Vector4 &v) {
		x -= v.x; y -= v.y; z -= v.z, w -=v.w;
		return *this;
	}

	bool operator==(const Vector4 &v) const {
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	Vector4 operator*(double f) const {
		return Vector4(f*x, f*y, f*z, f*w);
	}
	
	Vector4 &operator*=(double f) {
		x *= f; y *= f; z *= f; w *= f;
		return *this;
	}

	Vector4 operator/(double f) const {
		assert(f!=0);
		double inv = 1.f / f;
		return Vector4(x * inv, y * inv, z * inv, w * inv);
	}
	
	Vector4 &operator/=(double f) {
		assert(f!=0);
		double inv = 1.f / f;
		x *= inv; y *= inv; z *= inv; w *= inv;
		return *this;
	}

	Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	double &operator[](int i) {
		assert(i >= 0 && i <= 3);
		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
		}
	} 

	double dot(const Vector4 &v) const {
		return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	Vector4 componentMul(const Vector4 &v) const {
		return Vector4(x*v.x, y*v.y, z*v.z, w*v.w);
	}

	Vector4 clamp01() { 
		if (x>1.f) x=1.f;
			else if (x<0.f) x=0.f;
		if (y>1.f) y=1.f;
			else if (y<0.f) y=0.f;
		if (z>1.f) z=1.f;
			else if (z<0.f) z=0.f;
		if (w>1.f) w=1.f;
			else if (w<0.f) w=0.f;
		return *this;
	}

	double lengthSquared() const { return x*x + y*y + z*z + w*w; }
	
	double length() const { return sqrt(lengthSquared()); }

	// Vector4 Data
	double x, y, z, w;
};


#endif //_VECTOR4_H
