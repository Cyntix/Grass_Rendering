/****************************************************************************
|*  Vector3.h
|*
|*  Definition of a 3d double vector and its basic functionality.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Balint Miklos, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _VECTOR3_H
#define _VECTOR3_H


#include <math.h>
#include <assert.h>


class Vector3 {
public:
	// Vector3 Methods
	Vector3(double _x=0, double _y=0, double _z=0)
		: x(_x), y(_y), z(_z) {
	}

	Vector3 operator+(const Vector3 &v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	
	Vector3& operator+=(const Vector3 &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector3 operator-(const Vector3 &v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	
	Vector3& operator-=(const Vector3 &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	bool operator==(const Vector3 &v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	Vector3 operator*(double f) const {
		return Vector3(f*x, f*y, f*z);
	}
	
	Vector3 &operator*=(double f) {
		x *= f; y *= f; z *= f;
		return *this;
	}

	Vector3 operator/(double f) const {
		assert(f!=0);
		double inv = 1.f / f;
		return Vector3(x * inv, y * inv, z * inv);
	}
	
	Vector3 &operator/=(double f) {
		assert(f!=0);
		double inv = 1.f / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	double &operator[](int i) {
		assert(i >= 0 && i <= 2);
		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
		}
		return z;
	} 

	const double &operator[](int i) const {
		assert(i >= 0 && i <= 2);
		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return z;
		}
	}


	Vector3 cross(const Vector3 &v) const{
		return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y * v.x);
	}

	double dot(const Vector3 &v) const{
		return x*v.x + y*v.y + z*v.z;
	}

	Vector3 normalize() { 
		double l = length();
		assert(l != 0);
		x /= l;
		y /= l;
		z /= l;
		return *this;
	}

	Vector3 clamp01() { 
		if (x>1.f) x=1.f;
			else if (x<0.f) x=0.f;
		if (y>1.f) y=1.f;
			else if (y<0.f) y=0.f;
		if (z>1.f) z=1.f;
			else if (z<0.f) z=0.f;
		return *this;
	}

	double lengthSquared() const { return x*x + y*y + z*z; }
	
	double length() const { return sqrt(lengthSquared()); }

	// Vector3 Data
	double x, y, z;
};

inline Vector3 operator*(double f, const Vector3 & o) {
	return o*f;
}



#endif //_VECTOR3_H
