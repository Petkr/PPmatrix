#pragma once

namespace PPmatrix
{
	template <typename T, typename U>
	requires !requires (const T t, const U u) { t + u; }
	constexpr auto operator+(T t, const U& u)
	{
		t += u;
		return t;
	}
	template <typename T, typename U>
	requires !requires (const T t, const U u) { t - u; }
	constexpr auto operator-(T t, const U& u)
	{
		t -= u;
		return t;
	}
	template <typename T, typename U>
	requires !requires (const T t, const U u) { t / u; }
	constexpr auto operator/(T t, const U& u)
	{
		t /= u;
		return t;
	}
	template <typename T, typename U>
	requires !requires (const T t, const U u) { t* u; }
	constexpr auto operator*(T t, const U& u)
	{
		t *= u;
		return t;
	}

	template <typename T>
	requires !requires (T t) { ++t; }
	constexpr auto& operator++(T& t)
	{
		t += 1;
		return t;
	}
	template <typename T>
	requires !requires (T t) { --t; }
	constexpr auto& operator--(T& t)
	{
		t -= 1;
		return t;
	}
}
