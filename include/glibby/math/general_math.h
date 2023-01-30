#pragma once

#include "glibby/primitives/point2D.h"

#include <iostream>

namespace glibby
{
	#define FLT_NEAR_ZERO 0.0001f

	template <typename T, size_t n>
	struct Vec
	{
		T data[n];

		Vec() { memset(&data, 0, n * sizeof(T)); }
		explicit Vec(T all) { for (size_t i = 0; i < n; i++) data[i] = all; }
		explicit Vec(T arr[n]) { for (size_t i = 0; i < n; i++) data[i] = arr[i]; }

		T& operator[](size_t idx) { return data[idx]; }
		T  operator[](size_t idx) const { return data[idx]; }
	};

	typedef Vec<float, 2> Vec2;
	typedef Vec<float, 3> Vec3;
	typedef Vec<float, 4> Vec4;
	typedef Vec<double, 2> Vec2d;
	typedef Vec<double, 3> Vec3d;
	typedef Vec<double, 4> Vec4d;
	typedef Vec<uint32_t, 2> Vec2i;
	typedef Vec<uint32_t, 3> Vec3i;
	typedef Vec<uint32_t, 4> Vec4i;

	template <typename T>
	struct Vec<T, 2> {
		union {
			T data[2];
			struct { T x, y; };
			struct { T r, g; };
		};

		Vec() : x(0), y(0) {}
		explicit Vec(T all) : x(all), y(all) {}
		Vec(T x, T y) : x(x), y(y) {}
		Vec(const Vec3& other) : x(other.x), y(other.y) {}
		Vec(const Vec4& other) : x(other.x), y(other.y) {}
		Vec(const Point2D& other) : x(other.x), y(other.y) {}
		Vec(const Point3D& other) : x(other.x), y(other.y) {}
	};

	template <typename T>
	struct Vec<T, 3> {
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			Vec<T, 2> xy;
			Vec<T, 2> rg;
		};

		Vec() : x(0), y(0), z(0) {}
		explicit Vec(T all) : x(all), y(all), z(all) {}
		Vec(T x, T y, T z) : x(x), y(y), z(z) {}
		Vec(const Vec2& other) : x(other.x), y(other.y), z(0.0f) {}
		Vec(const Vec4& other) : x(other.x), y(other.y), z(other.z) {}
		Vec(const Point2D& other) : x(other.x), y(other.y), z(0.0f) {}
		Vec(const Point3D& other) : x(other.x), y(other.y), z(other.z) {}
	};

	template <typename T>
	struct Vec<T, 4> {
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			Vec<T, 2> xy;
			Vec<T, 2> rg;
			Vec<T, 3> xyz;
			Vec<T, 3> rgb;
		};

		Vec() : x(0), y(0), z(0), w(0) {}
		explicit Vec(T all) : x(all), y(all), z(all), w(all) {}
		Vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec(const Vec2& other) : x(other.x), y(other.y), z(0.0f), w(0.0f) {}
		Vec(const Vec3& other) : x(other.x), y(other.y), z(other.z), w(0.0f) {}
		Vec(const Point2D& other) : x(other.x), y(other.y), z(0.0f), w(0.0f) {}
		Vec(const Point3D& other) : x(other.x), y(other.y), z(other.z), w(0.0f) {}
	};

	template<typename T, size_t n>
	Vec<T, n> operator-(Vec<T, n> vec)
	{
		for (size_t i = 0; i < n; i++)
		{
			vec.data[i] = -vec.data[i];
		}
		return vec;
	}

	template<typename T, size_t n>
	Vec<T, n>& operator+=(Vec<T, n>& lhs, const Vec<T, n>& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] += rhs.data[i];
		}
		return lhs;
	}

	template<typename T, size_t n>
	Vec<T, n>& operator-=(Vec<T, n>& lhs, const Vec<T, n>& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] -= rhs.data[i];
		}
		return lhs;
	}

	template<typename T, size_t n>
	Vec<T, n>& operator*=(Vec<T, n>& lhs, T rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] *= rhs;
		}
		return lhs;
	}

	template<typename T, size_t n>
	Vec<T, n>& operator/=(Vec<T, n>& lhs, T rhs)
	{
		if (rhs == 0)
		{
			// TODO: LogError
			std::cerr << "Error: Attempted to normalize zero magnitude vector.\n";
			return lhs;
		}

		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] /= rhs;
		}
		return lhs;
	}

	template<typename T, size_t n>
	inline Vec<T, n> operator+(Vec<T, n> lhs, const Vec<T, n>& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] += rhs.data[i];
		}
		return lhs;
	}

	template<typename T, size_t n>
	inline Vec<T, n> operator-(Vec<T, n> lhs, const Vec<T, n>& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] -= rhs.data[i];
		}
		return lhs;
	}

	template<typename T, size_t n>
	inline Vec<T, n> operator*(Vec<T, n> lhs, float rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] *= rhs;
		}
		return lhs;
	}

	template<typename T, size_t n>
	inline Vec<T, n> operator*(float lhs, Vec<T, n> rhs)
	{
		return rhs * lhs;
	}

	template<typename T, size_t n>
	inline Vec<T, n> operator/(Vec<T, n> lhs, float rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			lhs.data[i] /= rhs;
		}
		return lhs;
	}

	template<typename T, size_t n>
	inline T Dot(const Vec<T, n>& lhs, const Vec<T, n>& rhs)
	{
		T sum = 0;
		for (size_t i = 0; i < n; i++)
		{
			sum += lhs.data[i] * rhs.data[i];
		}
		return sum;
	}

	template<typename T, size_t n>
	inline T SquareMagnitude(const Vec<T, n>& vec)
	{
		return Dot(vec, vec);
	}

	template<typename T, size_t n>
	inline T Magnitude(const Vec<T, n>& vec)
	{
		return sqrt(SquareMagnitude(vec));
	}

	template<typename T, size_t n>
	inline Vec<T, n> Normalize(const Vec<T, n>& vec)
	{
		float magnitude = Magnitude(vec);
		if (magnitude == 0)
		{
			// TODO: LogError
			std::cerr << "Error: Attempted to normalize zero magnitude vector.\n";
			return Vec<T, n>((T)0);
		}
		return vec / magnitude;
	}

	template<typename T, size_t n>
	inline bool operator==(const Vec<T, n>& lhs, const Vec<T, n>& rhs)
	{
		// TODO: account for floating point imprecision
		for (size_t i = 0; i < n; i++)
		{
			if (lhs.data[i] != rhs.data[i])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, size_t n>
	inline bool operator!=(const Vec<T, n>& lhs, const Vec<T, n>& rhs)
	{
		return !operator==(lhs, rhs);
	}

	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& os, const Vec<T, n>& vec)
	{
		os << "(" << vec.data[0];
		for (size_t i = 1; i < n; i++)
		{
			os << ", " << vec.data[i];
		}
		os << ")";
		return os;
	}
	
	template<typename T, size_t n>
	std::istream& operator>>(std::istream& is, Vec<T, n>& vec)
	{
		// TODO: set failbit
		//if (no valid object of T found in stream)
		//{
		//	is.setstate(std::ios::failbit);
		//}

		for (size_t i = 0; i < n; i++)
		{
			is >> vec.data[i];
		}
		return is;
	}

	template<typename T>
	Vec<T, 3> Cross(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
	{
		// | i  j  k  |
		// | x1 y1 z1 |
		// | x2 y2 z2 |

		T x = lhs.y * rhs.z - lhs.z * rhs.y;
		T y = lhs.x * rhs.z - lhs.z * rhs.x;
		T z = lhs.x * rhs.y - lhs.y * rhs.x;

		return Vec<T, 3>(x, -y, z);
	}

	template<size_t rows, size_t cols>
	struct Mat 
	{
		float data[rows][cols];
	};
}
