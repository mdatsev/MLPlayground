#pragma once

#include <array>
#include "utils.h"

template<typename Ty = float>
struct Point
{
public:
	Ty x;
	Ty y;
	Point(Ty x = 0, Ty y = 0)
		: x(x), y(y)
	{
	}
	std::array<Ty, 2> array() const
	{
		return { x, y };
	}
	bool operator == (const Point& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	Ty mappedX(Ty point_min, Ty point_max, Ty result_min, Ty result_max) const
	{
		return map(x, point_min, point_max, result_min, result_max);
	}
	Ty mappedY(Ty point_min, Ty point_max, Ty result_min, Ty result_max) const
	{
		return map(y, point_min, point_max, result_min, result_max);
	}
	void setMappedX(Ty value, Ty value_min, Ty value_max, Ty point_min, Ty point_max)
	{
		x = map(value, value_min, value_max, point_min, point_max);
	}
	void setMappedY(Ty value, Ty value_min, Ty value_max, Ty point_min, Ty point_max)
	{
		y = map(value, value_min, value_max, point_min, point_max);
	}
};

