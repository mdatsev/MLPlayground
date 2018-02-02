#pragma once
#include <cmath>
namespace activationFunctions
{
	template <typename Ty>
	Ty sign(Ty x)
	{
		return x >= 0 ? static_cast<Ty>(1) : static_cast<Ty>(-1);
	}
	template <typename Ty>
	Ty sigmoid(Ty x)
	{
		return 1 / (1 + std::exp(-x));
	}
}