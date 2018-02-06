#pragma once
#include <cmath>
template<typename Ty>
struct activationFunction
{
	using type = std::function<Ty(Ty)>;
	
	activationFunction(type activate, type derivative, type derivative_output)
		: activate(activate), derivative(derivative), derivative_output(derivative_output)
	{}

	const type activate;
	const type derivative;
	const type derivative_output;
};
namespace activationFunctions
{
	template <typename Ty>
	struct sigmoid : public activationFunction<Ty>
	{
		sigmoid()
			: activationFunction<Ty>(
				[](Ty x) { return 1 / (1 + std::exp(-x)); }, // activate
				[&](Ty x) { return this->activate(x) * (1 - this->activate(x)); }, // derivative //TODO: fix this
				[](Ty y) { return y * (1 - y); }) // derivative output
		{};
	};
	template <typename Ty>
	Ty sign(Ty x)
	{
		return x >= 0 ? static_cast<Ty>(1) : static_cast<Ty>(-1);
	}
	template <typename Ty>
	Ty step(Ty x)
	{
		return x > 0 ? static_cast<Ty>(1) : static_cast<Ty>(0);
	}
}