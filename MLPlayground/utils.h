#pragma once
template<typename Ty>
Ty map(Ty value, Ty in_min, Ty in_max, Ty out_min, Ty out_max)
{
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}