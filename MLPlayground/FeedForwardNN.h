#include <functional>
#include <array>
#include "Layer.h"

template<typename Ty, int nInputs, int nHidden, int nOutputs> //TODO: Have some fun with variadic tempaltes :)
class FeedForwardNN
{
public:
	using InputMatrix = Matrix<Ty, nInputs, 1>;
	using OutputMatrix = Matrix<Ty, nOutputs, 1>;
	Layer<Ty, nInputs, nHidden> hiddenLayer;
	Layer<Ty, nHidden, nOutputs> outputLayer;
	FeedForwardNN()
	{
	}
	FeedForwardNN(const activationFunction<Ty>& activationFunction)
		: hiddenLayer(activationFunction), outputLayer(activationFunction)
	{
	}
	template<typename RNG>
	void initWeights(RNG& rng)
	{
		hiddenLayer.weights.randomize(rng);
		outputLayer.weights.randomize(rng);
	}
	template<typename RNG>
	void initBiases(RNG& rng)
	{
		hiddenLayer.biases.randomize(rng);
		outputLayer.biases.randomize(rng);
	}
	auto Predict(Matrix<Ty, nInputs, 1> inputs)
	{
		return outputLayer.process(
			   hiddenLayer.process(
			   inputs));
	}
	void Train(InputMatrix inputs, OutputMatrix targets, Ty learning_rate)
	{
		auto hidden_outputs = hiddenLayer.process(inputs);
		auto outputs = outputLayer.process(hidden_outputs);

		auto output_errors = targets - outputs;
		auto output_gradients = outputs.map(outputLayer.activFunc.derivative_output);
		output_gradients *= output_errors;
		auto output_delta_weights = output_gradients * hidden_outputs.transposed();
		output_delta_weights *= learning_rate;
		outputLayer.weights += output_delta_weights;
		outputLayer.biases += output_gradients;

		auto hidden_errors = outputLayer.weights.transposed() * output_errors;
		auto hidden_gradients = hidden_outputs.map(outputLayer.activFunc.derivative_output);
		hidden_gradients *= hidden_errors;
		auto hidden_delta_weights = hidden_gradients * inputs.transposed();
		hidden_delta_weights *= learning_rate;
		hiddenLayer.weights += hidden_delta_weights;
		hiddenLayer.biases += hidden_gradients;
	}
};