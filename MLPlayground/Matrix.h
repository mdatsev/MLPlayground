#pragma once

#include <array>
#include <sstream>
#include <functional>

template<typename Ty, unsigned int rows, unsigned int cols>
class Matrix
{
	std::array<std::array<Ty, cols>, rows> data;
public:
	Matrix() {};
	Matrix(Ty fill_value)
	{
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				element = fill_value;
			}
		}
	}
	Matrix(const std::array<std::array<Ty, cols>, rows>& in)
		: data(in)
	{}
	auto transposed()
	{
		Matrix<Ty, cols, rows> result;
		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				result[i][j] = data[j][i];
			}
		}
		return result;
	}
	const wchar_t* createStringW()
	{
		std::wstringstream sstream;
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				sstream << element << "\t";
			}
			sstream << std::endl;
		}

		sstream.seekg(0, sstream.end);
		int length = sstream.tellg();
		sstream.seekg(0, sstream.beg);

		wchar_t * buffer = new wchar_t[length + 1];
		buffer[length] = 0;

		sstream.read(buffer, length);

		return buffer;
	}
	Matrix& fill(Ty scalar)
	{
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				element = scalar;
			}
		}
		return *this;
	}
	template<class RNG>
	Matrix& randomize(RNG& rng)
	{
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				element = rng();
			}
		}
		return *this;
	}
	Matrix& add(Ty scalar)
	{
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				element += scalar;
			}
		}
		return *this;
	}
	Matrix& operator+=(Ty scalar)
	{
		return add(scalar);
	}
	Matrix& add(Matrix& rhs)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] += rhs[i][j];
			}
		}
		return *this;
	}
	Matrix& operator+=(const Matrix& rhs)
	{
		return add(rhs);
	}
	Matrix& multiply(Ty scalar)
	{
		for (auto& row : data)
		{
			for (Ty& element : row)
			{
				element *= scalar;
			}
		}
		return *this;
	}
	Matrix& operator*=(Ty scalar)
	{
		return multiply(scalar);
	}
	template <unsigned int rhs_cols>
	auto operator*(const Matrix<Ty, cols, rhs_cols>& rhs) const
	{
		Matrix<Ty, rows, rhs_cols> result(0);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rhs_cols; j++)
			{
				for (int k = 0; k < cols; k++)
				{
					result[i][j] += data[i][k] * rhs[k][j];
				}
			}
		}
		return result;
	}
	auto& operator[](unsigned int row_index) const
	{
		return data[row_index];
	}
	auto& operator[](unsigned int row_index)
	{
		return data[row_index];
	}
	template<typename Ty>
	Matrix& apply(std::function<Ty(Ty)> f)
	{
		for (auto& row : data)
		{
			for (Ty& elem : row)
			{
				elem = f(elem);
			}
		}
		return *this;
	}
};

