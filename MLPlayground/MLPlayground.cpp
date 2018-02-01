// MLPlayground.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MLPlayground.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <iomanip>

#include "./Perceptron.h"
#include "./Point.h"

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
	double y = 0.3 * x + 0.2;
	return y;
}

std::vector<Point<double>> trainingInputs, testInputs;
std::vector<double> trainingOutputs, testOutputs;
std::default_random_engine generator(1202);
std::uniform_real_distribution<double> distribution(range.min, range.max);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	for (int i = 0; i < 5000; i++)
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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MLPLAYGROUND, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MLPLAYGROUND));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MLPLAYGROUND));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MLPLAYGROUND);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, 1, 1000 / 60, NULL);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

int i = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_TIMER:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, FALSE);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			HDC hMemDc = CreateCompatibleDC(hdc);
			RECT rect;
			GetClientRect(hWnd, &rect);
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
			auto y1 = min.mappedY(range.min, range.max, rect.top, rect.bottom);
			auto x2 = max.mappedX(range.min, range.max, rect.left, rect.right);
			auto y2 = max.mappedY(range.min, range.max, rect.top,  rect.bottom);
			MoveToEx(hMemDc, x1, y1, nullptr);
			LineTo(hMemDc, x2, y2);
			for (auto& t : trainingInputs)
			{
				SelectObject(hMemDc, perceptron.Predict(t.array()) == -1 ? rbrush : gbrush);
				auto x = t.mappedX(range.min, range.max, rect.left, rect.right);
				auto y = t.mappedY(range.min, range.max, rect.top, rect.bottom);
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

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
