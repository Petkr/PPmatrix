#pragma once
#include <utility>
#include <type_traits>

namespace PPmatrix
{
	template <typename T>
	constexpr std::size_t number_width(T number)
	{
		if constexpr (std::is_floating_point_v<T>)
			return 1;
		else
		{
			std::size_t width = 0;

			if constexpr (std::is_unsigned_v<T>)
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
	}

	namespace functor
	{
		constexpr auto number_width = [](auto&& x)
		{
			return PPmatrix::number_width(x);
		};
	}
}
