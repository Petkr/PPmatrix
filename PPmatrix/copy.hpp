#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	constexpr void copy(view auto&& from, view auto&& to)
	{
		zip(from, to,
			[](const auto& from, auto& to)
			{
				to = from;
			});
	}

	constexpr void copy(const std::initializer_list<auto>& l, view auto&& to)
	{
		copy(detail::wrap_initializer_list(l), to);
	}

	namespace functor
	{
		constexpr auto copy = [](view auto&& from, view auto&& to)
		{
			PPmatrix::copy(from, to);
		};
	}
}
