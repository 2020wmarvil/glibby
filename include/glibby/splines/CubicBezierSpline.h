#include "glibby/primitives/point.h"
namespace glibby
{
	template<typename T, int N>
	class CubicBezierSpline
	{
	public:
		CublicBezierSpline(const Point<T, N>[4] sourcepoints)
		{
			for(int i = 0; i < 4; i++)
			{
				P[i] = sourcepoints[i];
			}
		}
		Point<T,N> SampleAt(const T& t)
		{
			Point<T, N> returnPoint;
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					returnPoint.points[i] =
						P[j].points[i] * (1 - t) * (1 - t) * (1 - t) +
						P[j].points[i] * 3 * (1 - t) * (1 - t) * t +
						P[j].points[i] * 3 * (1 - t) * t * t +
						P[j].points[i] * t * t * t;
				}
			}
			return returnPoint;
		}
		Point<T, N> SampleAtUniform(const T& t)
		{
			Point<T, N> returnPoint;
			Point<T, N> point1;
			Point<T, N> point2;
			T distance;
			T distances[20];
			T totalDistance = 0;
			T targetDistance;

			for (int i = 0; i < 20; i++)
			{
				point1 = sampleAt(i / 20);
				point2 = sampleAt((i + 1) / 20);

				distance = EuclideanDistance(point1, point2);
				distances[i] = distance;
				totalDistance += distance;
			}

			distance = 0;
			int i = 0;
			targetDistance = t * totalDistance;

			while (distance < targetDistance)
			{
				distance += distances[i];
				i++;
			}

			returnPoint = sampleAt(i / 20);

			return returnPoint;
		}
		Point<T, N> getPoint(int i)
		{
			return P[i];
		}
	private: 
		Point<T, N> P[4];
	};
}
