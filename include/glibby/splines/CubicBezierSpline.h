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
				points[i] = sourcepoints[i];
			}
		}
		T sampleAt(const T& at);

	private: 
		Point<T, N> points[4];
	};
}