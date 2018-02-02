#pragma once

#include "resource.h"

#include "Perceptron.h"
#include "Point.h"
#include "Matrix.h"
#include "FeedForwardNN.h"

using namespace std;

Perceptron<double, 2> perceptron;

template <typename Ty>
double avgError(std::vector<Point<Ty>> inputs, std::vector<double> outputs)
{
	double sum = 0;
	for (int i = 0; i < inputs.size(); i++)
	{
		sum += abs(outputs[i] - perceptron.Predict({ inputs[i].x, inputs[i].y }));
	}
	return sum / inputs.size();
}

const struct { double min = -1; double max = 1; } range;

double f(double x)
{
	double y = 0.89 * x - 0.1;
	return y;
}

std::vector<Point<double>> trainingInputs, testInputs;
std::vector<double> trainingOutputs, testOutputs;
std::default_random_engine generator(1202);
std::uniform_real_distribution<double> distribution(range.min, range.max);

class MLPlayground
{
public:
	void setup()
	{
		auto nn = FeedForwardNN<float, 2, 2, 1>(activationFunctions::step<float>);
		Matrix<float, 2, 1> false1({ { { 0 }, { 0 } } });
		Matrix<float, 2, 1> false2({ { { 1 }, { 1 } } });
		Matrix<float, 2, 1> true1( { { { 1 }, { 0 } } });
		Matrix<float, 2, 1> true2( { { { 0 }, { 1 } } });
		nn.hiddenLayer.weights = Matrix<float, 2, 2>({ { {  1  ,  1   },
			                                             { -1 ,  -1  } } });
		nn.hiddenLayer.biases = Matrix<float, 2, 1>({ { { -.5 },  
														{ 1.5 } } });
		nn.outputLayer.weights = Matrix<float, 1, 2>({ { { 1, 1 } } });
		nn.outputLayer.biases = Matrix<float, 1, 1>({ { { -1.5 } } });
		nn.feedForward(false1).printDebugString();
		nn.feedForward(false2).printDebugString();
		nn.feedForward(true1).printDebugString();
		nn.feedForward(true1).printDebugString();

		/*for (int i = 0; i < 100; i++)
		{
		double x = distribution(generator), y = distribution(generator);
		trainingInputs.emplace_back(x, y);
		trainingOutputs.push_back(y > f(x) ? 1.0 : -1.0);
		}

		for (int i = 0; i < 5000; i++)
		{
		double x = distribution(generator), y = distribution(generator);
		testInputs.emplace_back(x, y);
		testOutputs.push_back(y > f(x) ? 1.0 : -1.0);
		}
		*/
	}
	int i = 0;
	void draw(HDC hdc, const RECT& rect)
	{
		

		/*HDC hMemDc = CreateCompatibleDC(hdc);
		HBITMAP hBmp = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(hMemDc, hBmp);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hMemDc, &rect, hBrush);
		HBRUSH gbrush = CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH rbrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH bbrush = CreateSolidBrush(RGB(0, 0, 255));

		Point<double> min;
		Point<double> max;

		min.setMappedX(rect.left, rect.left, rect.right, range.min, range.max);
		max.setMappedX(rect.right, rect.left, rect.right, range.min, range.max);
		min.y = f(min.x);
		max.y = f(max.x);
		auto test = map(999, 0, 999, -1, 1);
		auto x1 = min.mappedX(range.min, range.max, rect.left, rect.right);
		auto y1 = min.mappedY(range.min, range.max, rect.bottom, rect.top);
		auto x2 = max.mappedX(range.min, range.max, rect.left, rect.right);
		auto y2 = max.mappedY(range.min, range.max, rect.bottom, rect.top);
		MoveToEx(hMemDc, x1, y1, nullptr);
		LineTo(hMemDc, x2, y2);
		for (auto& t : trainingInputs)
		{
		SelectObject(hMemDc, perceptron.Predict(t.array()) == -1 ? rbrush : gbrush);
		auto x = t.mappedX(range.min, range.max, rect.left, rect.right);
		auto y = t.mappedY(range.min, range.max, rect.bottom, rect.top);
		Ellipse(hMemDc, x, y, x + 10, y + 10);
		}
		for (auto& t : testInputs)
		{
		SelectObject(hMemDc, perceptron.Predict(t.array()) == -1 ? rbrush : gbrush);
		//Ellipse(hMemDc, t.x, t.y, t.x + 10, t.y + 10);
		}
		DeleteObject(gbrush);
		DeleteObject(rbrush);

		for (int j = 0; j < 1; j++)
		{
		i = i < trainingInputs.size() - 1 ? i + 1 : 0;
		{
		auto a = trainingInputs[i].array();
		auto b = trainingOutputs[i];
		perceptron.Train(a, b, 0.01);
		}
		}

		SetTextColor(hMemDc, RGB(0, 0, 255));
		SetBkMode(hMemDc, TRANSPARENT);
		TCHAR buffer[80];
		_stprintf_s(buffer, _T("err:%lf"), avgError(testInputs, testOutputs));
		TextOut(hMemDc, 0, 0, buffer, _tcslen(buffer));

		BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hMemDc, 0, 0, SRCCOPY);

		DeleteDC(hMemDc);
		DeleteObject(hBmp);
		DeleteObject(hBrush);
		*/
	}
};