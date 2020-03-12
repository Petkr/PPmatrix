#pragma once
#include <utility>
#include <type_traits>

namespace PPmatrix
{
	// calculates the width of a number when printed

	consteval size_t number_width(std::floating_point auto number)
	{
		return 1;
	}
	template <std::integral T>
	constexpr size_t number_width(T number)
	{
		size_t width = 0;

		if constexpr (std::unsigned_integral<T>)
		{
			if (number == 0)
				++width;
		}
		else
		{
			if (number <= 0)
				++width;
		}

		while (number != 0)
		{
			number /= 10;
			++width;
		}

		return width;
	}

	namespace functor
	{
		constexpr auto number_width = [](auto&& x)
		{
			return PPmatrix::number_width(x);
		};
	}
}
