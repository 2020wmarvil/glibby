#pragma once

#include <iostream>

namespace glibby
{
	/* TODO:
	 * vec2, vec3, vec4
	 * vec2d, vec3d, vec4d
	 * vector swizzling
	 * mat2x2, mat3x3, mat4x4, matMxN
	 * quaternion
	 * complex number
	 * simd/intrinsics
	 * 
	 */

	#define FLT_NEAR_ZERO 0.0001f

	struct Vec2
	{
		union
		{
			struct
			{
				float x;
				float y;
			};

			struct
			{
				float r;
				float g;
			};

			struct
			{
				float u;
				float v;
			};
		};

		Vec2() : x(0.0f), y(0.0f) {}
		Vec2(float xy) : x(xy), y(xy) {}
		Vec2(float x, float y) : x(x), y(y) {}

		Vec2 operator-()
		{
			return Vec2(-x, -y);
		}

		Vec2& operator+=(const Vec2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		Vec2& operator-=(const Vec2& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		Vec2& operator*=(float rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			return *this;
		}

		Vec2& operator/=(float rhs)
		{
			this->x /= rhs;
			this->y /= rhs;
			return *this;
		}

		float Magnitude()
		{
			return std::sqrt(this->Dot(*this));
		}

		float SquareMagnitude()
		{
			return this->Dot(*this);
		}

		Vec2 Normalized()
		{
			float magnitude = this->Magnitude();

			if (magnitude == 0)
			{
				// TODO: error/warning
				return Vec2(0.0f);
			}

			return *this / magnitude;
		}

		void Normalize()
		{
			float magnitude = this->Magnitude();

			if (magnitude == 0)
			{
				// TODO: error/warning
				*this = Vec2(0.0f);
			}
			else
			{
				*this /= magnitude;
			}
		}

		float Dot(const Vec2& rhs)
		{
			return this->x * rhs.x + this->y * rhs.y;
		}

		Vec3 Cross(const Vec2& rhs)
		{
			return Vec3(0.0f, 0.0f, this->x * rhs.y - this->y * rhs.x);
		}
	};

	inline Vec2 operator+(Vec2 lhs, const Vec2& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	inline Vec2 operator-(Vec2 lhs, const Vec2& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	// TODO: should this be const&?
	inline Vec2 operator*(Vec2 lhs, float rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	inline Vec2 operator*(float lhs, const Vec2& rhs)
	{
		return rhs * lhs;
	}

	inline Vec2 operator/(Vec2 lhs, float rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	inline bool operator==(const Vec2& lhs, const Vec2& rhs)
	{
		// TODO: account for floating point imprecision
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	inline bool operator!=(const Vec2& lhs, const Vec2& rhs)
	{
		return !operator==(lhs,rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& obj)
	{
		os << "(" << obj.x << ", " << obj.y << ")";
		return os;
	}
	
	std::istream& operator>>(std::istream& is, Vec2& obj)
	{
		// TODO: set failbit
		//if ( /* no valid object of T found in stream */)
		//{
		//	is.setstate(std::ios::failbit);
		//}

		is >> obj.x >> obj.y;
		return is;
	}

	struct Vec3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			struct
			{
				float r;
				float g;
				float b;
			};

			struct
			{
				float u;
				float v;
				float s;
			};
		};

		Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vec3(const Vec2& other) : x(other.x), y(other.y), z(0.0f) {}

		Vec3 operator-()
		{
			return Vec3(-x, -y, -z);
		}

		Vec3& operator+=(const Vec3& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		Vec3& operator-=(const Vec3& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		Vec3& operator*=(float rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			this->z *= rhs;
			return *this;
		}

		Vec3& operator/=(float rhs)
		{
			this->x /= rhs;
			this->y /= rhs;
			this->z /= rhs;
			return *this;
		}

		float Magnitude()
		{
			return std::sqrt(this->Dot(*this));
		}

		float SquareMagnitude()
		{
			return this->Dot(*this);
		}

		Vec3 Normalized()
		{
			float magnitude = this->Magnitude();

			if (magnitude == 0)
			{
				// TODO: error/warning
				return Vec3(0.0f);
			}

			return *this / magnitude;
		}

		void Normalize()
		{
			float magnitude = this->Magnitude();

			if (magnitude == 0)
			{
				// TODO: error/warning
				*this = Vec3(0.0f);
			}
			else
			{
				*this /= magnitude;
			}
		}

		float Dot(const Vec3& rhs)
		{
			return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
		}

		Vec3 Cross(const Vec3& rhs)
		{
			// | i  j  k  |
			// | x1 y1 z1 |
			// | x2 y2 z2 |

			float x = this->y * rhs.z - this->z * rhs.y;
			float y = this->x * rhs.z - this->z * rhs.x;
			float z = this->x * rhs.y - this->y * rhs.x;

			return Vec3(x, y, z);
		}
	};

	inline Vec3 operator+(Vec3 lhs, const Vec3& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	inline Vec3 operator-(Vec3 lhs, const Vec3& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	// TODO: should this be const&?
	inline Vec3 operator*(Vec3 lhs, float rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	inline Vec3 operator*(float lhs, const Vec3& rhs)
	{
		return rhs * lhs;
	}

	inline Vec3 operator/(Vec3 lhs, float rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	inline bool operator==(const Vec3& lhs, const Vec3& rhs)
	{
		// TODO: account for floating point imprecision
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	inline bool operator!=(const Vec3& lhs, const Vec3& rhs)
	{
		return !operator==(lhs,rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Vec3& obj)
	{
		os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
		return os;
	}
	
	std::istream& operator>>(std::istream& is, Vec3& obj)
	{
		// TODO: set failbit
		//if ( /* no valid object of T found in stream */)
		//{
		//	is.setstate(std::ios::failbit);
		//}

		is >> obj.x >> obj.y >> obj.z;
		return is;
	}
}
