#include <noise_functions.h>
namespace glibby
{
	std::vector<float> whiteNoise1d(float i)
	{
		std::vector<float> returnVector(1);

		returnVector[0] = (4.9482 * i - (int)(4.9482 * i));
	}
	std::vector<float> whiteNoise2d(float i, float j)
	{
		std::vector<float> returnVector(2);

		returnVector[0] = (4.9482 * i - (int)(4.9482 * i));
		returnVector[1] = (5.3267 * i - (int)(5.3267 * i));
	}
}