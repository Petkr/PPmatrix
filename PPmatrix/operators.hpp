#pragma once
#include <cstddef>

namespace PPmatrix
{
	template <typename T>
	constexpr auto operator+(T t, std::size_t u)
	{
		t += u;
		return t;
	}
	template <typename T>
	constexpr auto operator-(T t, std::size_t u)
	{
		t -= u;
		return t;
	}
	template <typename T>
	constexpr auto& operator++(T& t)
	{
		t += 1;
		return t;
	}
	template <typename T>
	constexpr auto& operator--(T& t)
	{
		t -= 1;
		return t;
	}
}