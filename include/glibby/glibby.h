#pragma once

namespace glibby
{
	struct Point2D
	{
		float x;
		float y;
	};

	float Distance(const Point2D& p1, const Point2D& p2);
}