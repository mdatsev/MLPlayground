#pragma once

#include <array>
#include <functional>
#include <random>
#include "activationFunctions.h"
template<typename Ty = float, int nInputs = 2>
class Perceptron
{
	using inputVector = std::array<Ty, nInputs>;
	inputVector weights;
	Ty bias;
	std::function<Ty(Ty)> activFunc;
public:
	Perceptron(const std::function<Ty(Ty)>& activationFunction = activationFunctions::sign<Ty>)
		: activFunc(activationFunction)
	{
		std::default_random_engine generator(1202);
		std::uniform_real_distribution<Ty> distribution(-1.0, 1.0);
		for (Ty& w : weights)
		{
			w = distribution(generator);
		}
	}
	~Perceptron() 
	{
	}
	Ty Predict(inputVector inputs)
	{
		Ty sum = bias;
		for (int i = 0; i < nInputs; i++)
		{
			sum += inputs[i] * weights[i];
		}
		return activFunc(sum);
	}

	void Train(inputVector inputs, Ty target, Ty learningRate)
	{
		Ty error = target - Predict(inputs);
		for (int i = 0; i < nInputs; i++)
		{
			weights[i] += error * inputs[i] * learningRate;
			bias += error * learningRate;
		}
	}
};

