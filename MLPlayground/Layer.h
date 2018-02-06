#pragma once
#include "Matrix.h"
#include "activationFunctions.h"

template<typename Ty, int nInputs, int nOutputs>
struct Layer
{
	Matrix<Ty, nOutputs, nInputs> weights;
	Matrix<Ty, nOutputs, 1> biases;
	const activationFunction<Ty> activFunc;
	Layer(const activationFunction<Ty>& activationFunction)// = activationFunctions::sigmoid<Ty>)
		: activFunc{ activationFunction }
	{
	}
	auto process(Matrix<Ty, nInputs, 1> inputs)
	{
		Matrix<Ty, nOutputs, 1> result;
		result = weights * inputs;
		result += biases;
		result.apply(activFunc.activate);
		return result;
	}
};
