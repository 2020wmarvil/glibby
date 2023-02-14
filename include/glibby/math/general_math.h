#pragma once

#include "glibby/primitives/point.h"

#include <cmath>
#include <cstring>
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
		Vec(const Vec<T, 3>& other) : x(other.x), y(other.y) {}
		Vec(const Vec<T, 4>& other) : x(other.x), y(other.y) {}
		Vec(const Point2& other) : x(other.points[0]), y(other.points[1]) {}
		Vec(const Point3& other) : x(other.points[0]), y(other.points[1]) {}
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
		Vec(const Vec<T, 2>& other) : x(other.x), y(other.y), z(0.0f) {}
		Vec(const Vec<T, 4>& other) : x(other.x), y(other.y), z(other.z) {}
		Vec(const Point2& other) : x(other.points[0]), y(other.points[1]), z(0.0f) {}
		Vec(const Point3& other) : x(other.points[0]), y(other.points[1]), z(other.points[2]) {}
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
		Vec(const Vec<T, 2>& other) : x(other.x), y(other.y), z(0.0f), w(0.0f) {}
		Vec(const Vec<T, 3>& other) : x(other.x), y(other.y), z(other.z), w(0.0f) {}
		Vec(const Point2& other) : x(other.points[0]), y(other.points[1]), z(0.0f), w(0.0f) {}
		Vec(const Point3& other) : x(other.points[0]), y(other.points[1]), z(other.points[2]), w(0.0f) {}
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
			std::cerr << "Error: Attempted to divide vector by zero.\n";
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


	template<typename T, size_t n>
	struct MAT
	{
		T data[n][n];
		size_t size = n;
		MAT()
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					data[i][j] = 0;
				}
			}
		}
		MAT(T all)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					data[i][j] = all;
				}
			}
		}
		MAT(T mat[n][n])
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					data[i][j] = mat[i][j];
				}
			}
		}
		MAT(const MAT<T, n>& copy)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					data[i][j] = copy.data[i][j];
				}
			}
		}
		MAT<T, n>& operator =(const MAT<T, n>& rhs)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					data[i][j] = rhs.data[i][j];
				}
			}
			return *this;
		}
	};
	template <typename T>
	using MAT2x2 = MAT<T, 2>;
	template <typename T>
	using MAT3x3 = MAT<T, 3>;
	template <typename T>
	using MAT4x4 = MAT<T, 4>;

	template<typename T, size_t n>
	void print_matrix(const MAT<T, n>& mat)
	{

		for (int i = 0; i < n; i++)
		{
			if (i == 0)std::cout << "[";
			else std::cout << " ";
			for (int j = 0; j < n; j++)
			{
				std::cout << mat.data[i][j];
				if (j != n - 1)std::cout << " ";
			}
			if (i == n - 1)std::cout << "]";
			else std::cout << " ";
			std::cout << std::endl;
		}

	}
	template<typename T, size_t n>
	MAT<T, n> scalar_multiplication(float coefficient, const MAT<T, n>& mat)
	{
		MAT<T, n> result(mat);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result.data[i][j] *= coefficient;
			}
		}
		return result;
	}
	template<typename T, size_t n>
	MAT<T, n> scalar_division(float divisor, const MAT<T, n>& mat)
	{
		MAT<T, n> result(mat);
		if (divisor == 0)
		{
			std::cout << "Cannot divide by 0." << std::endl;
			exit(1);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result.data[i][j] /= divisor;
			}
		}
		return result;
	}
	template <typename T, size_t n>
	MAT<T, n> matrix_addition(const MAT<T, n>& mat1, const MAT<T, n>& mat2)
	{
		MAT<T, n> result;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
			}
		}
		return result;
	}
	template <typename T, size_t n>
	MAT<T, n> matrix_subtraction(const MAT<T, n>& mat1, const MAT<T, n>& mat2)
	{
		MAT<T, n> result;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
			}
		}
		return result;
	}
	template <typename T, size_t n>
	MAT<T, n> matrix_multiplication(const MAT<T, n>& mat1, const MAT<T, n>& mat2)
	{
		MAT<T, n> result;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				for (int k = 0; k < n; k++)
				{
					result.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
				}
			}
		}
		return result;
	}
	template <typename T, size_t n>
	MAT<T, n> matrix_transposition(const MAT<T, n>& mat1)
	{
		MAT<T, n> result;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				result.data[i][j] = mat1.data[j][i];
			}
		}
		return result;
	}
	template <typename T, size_t n>
	float matrix_determinant(const MAT<T, n>& mat)
	{
		float result;
		if (n == 2)
		{
			result = (mat.data[0][0] * mat.data[1][1] - mat.data[0][1] * mat.data[1][0]);
		}
		if (n == 3)
		{
			MAT<T, 2> temp2x2_1;
			MAT<T, 2> temp2x2_2;
			MAT<T, 2> temp2x2_3;
			int count1 = 0, count2 = 0, count3 = 0;
			for (int j = 0; j < 3; j++)
			{
				for (int i = 0; i < 2; i++)
				{
					if (j == 0)
					{
						temp2x2_2.data[i][count2] = mat.data[i + 1][j];
						temp2x2_3.data[i][count3] = mat.data[i + 1][j];
					}
					if (j == 1)
					{
						temp2x2_1.data[i][count1] = mat.data[i + 1][j];
						temp2x2_3.data[i][count3] = mat.data[i + 1][j];
					}
					if (j == 2)
					{
						temp2x2_1.data[i][count1] = mat.data[i + 1][j];
						temp2x2_2.data[i][count2] = mat.data[i + 1][j];
					}
				}
				if (j == 0)count2++; count3++;
				if (j == 1)count1++;
			}
			result = mat.data[0][0] * matrix_determinant(temp2x2_1)
				- mat.data[0][1] * matrix_determinant(temp2x2_2)
				+ mat.data[0][2] * matrix_determinant(temp2x2_3);
		}
		if (n == 4)
		{
			MAT<T, 3> temp3x3_1;
			MAT<T, 3> temp3x3_2;
			MAT<T, 3> temp3x3_3;
			MAT<T, 3> temp3x3_4;
			int count1 = 0, count2 = 0, count3 = 0, count4 = 0;
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 3; i++)
				{
					if (j == 0)
					{
						temp3x3_2.data[i][count2] = mat.data[i + 1][j];
						temp3x3_3.data[i][count3] = mat.data[i + 1][j];
						temp3x3_4.data[i][count4] = mat.data[i + 1][j];
					}
					if (j == 1)
					{
						temp3x3_1.data[i][count1] = mat.data[i + 1][j];
						temp3x3_3.data[i][count3] = mat.data[i + 1][j];
						temp3x3_4.data[i][count4] = mat.data[i + 1][j];
					}
					if (j == 2)
					{
						temp3x3_1.data[i][count1] = mat.data[i + 1][j];
						temp3x3_2.data[i][count2] = mat.data[i + 1][j];
						temp3x3_4.data[i][count4] = mat.data[i + 1][j];
					}
					if (j == 3)
					{
						temp3x3_1.data[i][count1] = mat.data[i + 1][j];
						temp3x3_2.data[i][count2] = mat.data[i + 1][j];
						temp3x3_3.data[i][count3] = mat.data[i + 1][j];
					}
				}
				if (j == 0)count2++; count3++; count4++;
				if (j == 1)count1++; count3++; count4++;
				if (j == 2)count1++; count2++; count4++;
			}
			result = mat.data[0][0] * matrix_determinant(temp3x3_1)
				- mat.data[0][1] * matrix_determinant(temp3x3_2)
				+ mat.data[0][2] * matrix_determinant(temp3x3_3)
				- mat.data[0][3] * matrix_determinant(temp3x3_4);
		}
		return result;
	}