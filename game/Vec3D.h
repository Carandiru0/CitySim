#pragma once

#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

/* 3D Vector Class */

template <class T>
class Vec3D {
	public:
		Vec3D() {
			x = 0;
			y = 0;
			z = 0;
		}

		Vec3D(T x, T y, T z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline void setX(T x) { this->x = x; }
		inline void setY(T y) { this->y = y; }
		inline void setZ(T z) { this->z = z; }

		inline T getX() { return x; }
		inline T getY() { return y; }
		inline T getZ() { return z; }

		inline void operator+=(T k) {
			x += k;
			y += k;
			z += k;
		}

		inline void operator+=(Vec3D v) {
			x += v.getX();
			y += v.getY();
			z += v.getZ();
		}

		inline void operator-=(T k) {
			x -= k;
			y -= k;
			z -= k;
		}

		inline void operator-=(Vec3D v) {
			x -= v.getX();
			y -= v.getY();
			z -= v.getZ();
		}

		inline Vec3D operator+(T k) { return Vec3D(x + k, y + k, z + k); }
		inline Vec3D operator+(Vec3D v) { return Vec3D(x + v.getX(), y + v.getY(), z + v.getZ()); }

		inline Vec3D operator-(T k) { return Vec3D(x - k, y - k, z - k); }
		inline Vec3D operator-(Vec3D v) { return Vec3D(x - v.getX(), y - v.getY(), z - v.getZ()); }

		inline Vec3D operator*(T k) { return Vec3D(x * k, y * k, z * k); }
		inline T operator*(Vec3D v) { return x * v.getX() + y * v.getY() + z * v.getZ(); }

		inline Vec3D operator/(T k) { return Vec3D(x / k, y / k, z / k); }
		inline Vec3D operator/(Vec3D v) { return Vec3D(x / v.getX(), y / v.getY(), z / v.getZ()); }

		inline float magnitude() { return sqrtf(x * x + y * y + z * z); }

		inline Vec3D normalised() {
			float m = magnitude();

			if (m == 0.f)
				return Vec3D();

			return Vec3D(x / m, y / m, z / m);
		}

		inline float distance(Vec3D v) {
			float dx = x - v.getX();
			float dy = y - v.getY();
			float dz = z - v.getZ();

			return sqrtf(dx * dx + dy * dy + dz * dz);
		};

		inline Vec3D moveTowards(Vec3D v, float vel) {
			return (v - *this).normalised() * vel;
		}

		inline Vec3D cross(Vec3D v) {
			Vec3D c;

			c.setX(y * v.getZ() - z * v.getY());
			c.setY(z * v.getX() - x * v.getZ());
			c.setZ(x * v.getY() - y * v.getX());

			return c;
		}

		inline float dot(Vec3D v) {
			float m = magnitude() + 1e-6f;

			return ((*this) * v) / (m * m);
		}

		bool collision(Vec3D initial, Vec3D target) {
			float lambda;
			Vec3D dir = target - initial;

			if (dir.getX() != 0)
				lambda = (getX() - initial.getX()) / dir.getX();
			else if (dir.getY() != 0)
				lambda = (getY() - initial.getY()) / dir.getY();
			else if (dir.getZ() != 0)
				lambda = (getZ() - initial.getZ()) / dir.getZ();
			else
				return true;

			if (lambda >= 1)
				return true;
		
			return false;
		}

		inline void rotate(float t, Vec3D o) {
			t *= M_PI / 180.0f;

			float nx = cosf(t) * (x - o.getX()) - sinf(t) * (z - o.getZ()) + o.getX();
			float nz = sinf(t) * (x - o.getX()) + cosf(t) * (z - o.getZ()) + o.getZ();

			x = nx, z = nz;
		}

		std::string tostring() {
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		}

	private:
		T x, y, z;
};