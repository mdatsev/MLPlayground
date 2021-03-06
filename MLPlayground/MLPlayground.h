#pragma once

#include "resource.h"

#include "Perceptron.h"
#include "Point.h"
#include "Matrix.h"
#include "FeedForwardNN.h"
#include <utility>

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
auto rng_neg_1_to_1 = std::bind(std::uniform_real_distribution<double>(-1, 1), generator);

template<typename Ty>
class AbstractFunc
{
public:
	virtual Ty operator()(Ty x) = 0;
};

template<typename Ty>
class Func : public AbstractFunc<Ty>
{
public:
	Ty operator()(Ty x) override
	{
		return 2.0f;
	}
};

template<typename Ty>
class AbstractFunc2
{
public:
	virtual Ty operator()(Ty x) = 0;
};

template <typename Ty>
class Func2 : public AbstractFunc2<Ty>
{
public:
	Ty operator()(Ty x) override
	{
		return 2.0f;
	}
};

template<typename Ty>
class M
{
public:
	void map(const std::function<Ty(Ty)>& f)
	{
		f(1.0f);
	}
};

template<typename Ty>
struct C
{
	AbstractFunc<Ty>* f_;
	template<typename ActivFuncTy>
	C(const ActivFuncTy& f)
		: f_(new ActivFuncTy(f))
	{
	}
	void m(Ty x)
	{
		M<Ty> m_;
		m_.map(std::ref(*f_));
	}
};

template<typename Ty>
struct C2
{
	AbstractFunc2<Ty>* f_;
	template<typename activFuncTy>
	C2(const activFuncTy& f)
		: f_(new activFuncTy(f))
	{
	}
	void m(Ty x)
	{
		M<Ty> m_;
		m_.map(std::ref(*f_));
	}
};
template <typename Ty>
struct abstractFuncs
{
	using type = std::function<Ty(Ty)>;
	abstractFuncs(type a, type b, type c)
		: m(a), m1(b), m2(c) {}
	type m;
	type m1;
	type m2;
};

template <typename Ty>
float test(const abstractFuncs<Ty>& f)
{
	return f.m(1);
}

class MLPlayground
{
public:
	FeedForwardNN<float, 2, 2, 1> nn = FeedForwardNN<float, 2, 2, 1>(activationFunctions::sigmoid<float>());
	std::vector<std::pair<Matrix<float, 2, 1>, Matrix<float, 1, 1>>> training_set = {
		{ { { { { 1 },{ 0 } } } },{ { 1 } } },
		{ { { { { 0 },{ 1 } } } },{ { 1 } } },
		{ { { { { 1 },{ 1 } } } },{ { 0 } } },
		{ { { { { 0 },{ 0 } } } },{ { 0 } } }
	};
	void setup()
	{
		nn.initWeights(rng_neg_1_to_1);
		nn.initBiases(rng_neg_1_to_1);
		/*nn.hiddenLayer.weights = Matrix<float, 2, 2>({ { { 1  ,  1 },
			{ -1 ,  -1 } } });
		nn.hiddenLayer.biases = Matrix<float, 2, 1>({ { { -.5 },
			{ 1.5 } } });
		nn.outputLayer.weights = Matrix<float, 1, 2>({ { { 1, 1 } } });
		nn.outputLayer.biases = Matrix<float, 1, 1>({ { { -1.5 } } });*/


		
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
		

		HDC hMemDc = CreateCompatibleDC(hdc);
		HBITMAP hBmp = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(hMemDc, hBmp);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hMemDc, &rect, hBrush);
		HBRUSH gbrush = CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH rbrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH bbrush = CreateSolidBrush(RGB(0, 0, 255));

		int divisions = 30;
		double segmentLenX = (rect.right - rect.left) / divisions;
		double segmentLenY = (rect.bottom - rect.top) / divisions;
		for (int i = 0; i < divisions; i++)
		{
			for (int j = 0; j < divisions; j++)
			{
				float x1 = map<float>(i, 0, divisions, 0, 1);
				float x2 = map<float>(j, 0, divisions, 0, 1);
				int v = map<float>(nn.Predict({ { { { x1 },{ x2 } } } })[0][0], 0, 1, 0, 255);
				HBRUSH brush = CreateSolidBrush(RGB(v, v, v));
				RECT rc = {
					rect.left + rect.right * ((float)i / divisions), 
					rect.top + rect.bottom * ((float)j / divisions),
					rc.left + segmentLenX,  rc.top + segmentLenY
				};
				FillRect(hMemDc, &rc, brush);
				DeleteObject(brush);
			}
		}

		/*MoveToEx(hMemDc, x1, y1, nullptr);
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
		}*/
		
		static double lr = 0.1;
		static int n = 100;
		for (int i = 0; i < n; i++)
		{
			std::uniform_int_distribution<int> ind(0, training_set.size() - 1); // guaranteed unbiased

			auto random_integer = ind(generator);
			nn.Train(training_set[random_integer].first, training_set[random_integer].second, lr);

		}

		SetTextColor(hMemDc, RGB(0, 255, 0));
		SetBkMode(hMemDc, TRANSPARENT);
		TCHAR buffer[80];
		_stprintf_s(buffer, _T("%s%s\n%s%s\n%lf"), 
			nn.Predict(training_set[0].first).createStringW(),
			nn.Predict(training_set[1].first).createStringW(),
			nn.Predict(training_set[2].first).createStringW(),
			nn.Predict(training_set[3].first).createStringW(),
			lr);
		//TextOut(hMemDc, 0, 0, buffer, _tcslen(buffer));
		RECT rc = rect;
		DrawText(hMemDc, buffer, -1, &rc, DT_WORDBREAK);
		BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hMemDc, 0, 0, SRCCOPY);

		DeleteDC(hMemDc);
		DeleteObject(hBmp);
		DeleteObject(hBrush);
		
	}

	void keyDown(WPARAM wParam, LPARAM lParam)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
		wchar_t buffer[100];
		swprintf_s(buffer, 100, L"%lu", p->vkCode);
		OutputDebugString(buffer);
	}
};