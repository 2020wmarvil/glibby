#include <"point.h">
namespace glibby
{
	template<typename T, int N>
	Point<T, N> WhiteNoise(Point<T, N> p)
	{
		Point<T, N> returnPoint;
		for (int i = 0; i < N; ++i)
		{
			returnPoint.points[i] = (i * 8.5121231 - ((int)i * 8.5121231));
		}
		return returnPoint;
	}
} 