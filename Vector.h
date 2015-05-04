#pragma once
#include <cmath>
#include <cassert>

struct Vector {
	double x, y, z, w;

	double& operator[](const size_t i) {
 	 	switch (i) {
 		case 0:
 			return x;
 		case 1:
 			return y;
 		case 2:
 			return z;
 		case 3:
 			return w;
 		default:
 		    assert(0);
 	 	}
	}

	double operator[](const size_t i) const {
 	 	switch (i) {
 		case 0:
 			return x;
 		case 1:
 			return y;
 		case 2:
 			return z;
 		case 3:
 			return w;
 		default:
 		    assert(0);
 	 	}
	}

	bool operator==(const Vector& v) const {
		return x==v.x && y==v.y && z==v.z && w==v.w;
	}

	Vector operator*(double a) const {
		return {a * x, a * y, a * z, w};
	}

	Vector operator*(const Vector& v) const {
		return {x * v.x, y * v.y, z * v.z, w * v.w};
	}

	Vector operator/(double d) const {
		return {x/d, y/d, z/d, w};
	}

	Vector operator+(const Vector& v) const {
		assert(w + v.w < 2);
		return {x + v.x, y + v.y, z + v.z, w + v.w};
	}

	Vector operator-() const {
		return {-x, -y, -z, w};
	}

	Vector operator-(const Vector& v) const {
		return {x - v.x, y - v.y, z - v.z, double(bool(w) != bool(v.w))};
	}

	double sum() const {
		return x + y + z;
	}

	double dot(const Vector& v) const {
		return (*this * v).sum();
	}

	double mag() const {
		return sqrt(x*x + y*y + z*z);
	}

	Vector norm() const {
		assert(this->mag() != 0);
		return *this / this->mag();
	}

	Vector cross(const Vector& v) const {
		assert(w == 0);
		assert(v.w == 0);
		return {
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x,
			w
		};
	}

	bool isClose(const Vector& v) const {
		double thresh = 0.0005;
		bool dx = std::abs(x - v.x) < thresh;
		bool dy = std::abs(y - v.y) < thresh;
		bool dz = std::abs(z - v.z) < thresh;
		bool dw = std::abs(w - v.w) < thresh;
		if (dx && dy && dz && dw) {
			return true;
		}
		return false;
	}
};

Vector operator*(double a, const Vector& v) {
	return {a * v.x, a * v.y, a * v.z, v.w};
}

struct Ray {
	Vector pos;
	Vector dir;
};