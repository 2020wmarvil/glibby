#include "glibby/primitives/point.h"
namespace glibby
{
	template<typename T, int N>
	class CubicBezierSpline
	{
	public:
		CublicBezierSpline(const Point<T, N>[4]& sourcepoints)
		{
			for{int i = 0; i < 4; i++}
			{
				P[i] = sourcepoints[i];
			}
		}
		Point<T,N> sampleAt(const T& t)
		{
			Point<T, N> returnPoint;
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					returnPoint.points[i] =
						(1 - t) * (1 - t) * (1 - t) * P[j].points[i] +
						3 * (1 - t) * (1 - t) * t * +
						3(1 - t) * t * t +
						t * t * t 
						//this is incomplete.
				}
			}
		}

	private: 
		Point<T, N> P[4];
	};
}
