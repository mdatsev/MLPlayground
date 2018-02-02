#pragma once
#include "Matrix.h"
#include "activationFunctions.h"

template<typename Ty, int nInputs, int nOutputs>
struct Layer
{
	Matrix<Ty, nOutputs, nInputs> weights;
	Matrix<Ty, nOutputs, 1> biases;
	std::function<Ty(Ty)> activFunc;
	Layer(const std::function<Ty(Ty)>& activationFunction = activationFunctions::sigmoid<Ty>)
		: activFunc(activationFunction)
	{
	}
	auto process(Matrix<Ty, nInputs, 1> inputs)
	{
		Matrix<Ty, nOutputs, 1> result;
		result = weights * inputs;
		result += biases;
		result.apply(activFunc);
		return result;
	}
};
