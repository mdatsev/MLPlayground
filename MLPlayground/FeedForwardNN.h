#include <functional>
#include <array>
#include "Layer.h"

template<typename Ty, int nInputs, int nHidden, int nOutputs> //TODO: Have some fun with variadic tempaltes :)
class FeedForwardNN
{
public:
	Layer<Ty, nInputs, nHidden> hiddenLayer;
	Layer<Ty, nHidden, nOutputs> outputLayer;
	FeedForwardNN()
	{
	}
	FeedForwardNN(const std::function<Ty(Ty)>& activationFunction)
		: hiddenLayer(activationFunction), outputLayer(activationFunction)
	{
	}
	auto feedForward(Matrix<Ty, nInputs, 1> inputs)
	{
		return outputLayer.process(
			   hiddenLayer.process(
			   inputs));
	}
};