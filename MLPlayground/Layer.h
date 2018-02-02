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
		//OutputDebugString(L"Inputs:\n");
		//inputs.printDebugString();

		Matrix<Ty, nOutputs, 1> result;
		result = weights * inputs;
		//OutputDebugString(L"W*I:\n");
		//result.printDebugString();

		result += biases;
		//OutputDebugString(L"W*I + B:\n");
		//result.printDebugString();

		result.apply(activFunc);
		//OutputDebugString(L"activated:\n");
		//result.printDebugString();
		//OutputDebugString(L"\n");
		return result;
	}
	};
