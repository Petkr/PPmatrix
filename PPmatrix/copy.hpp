#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	template <view ViewFrom, view ViewTo>
	constexpr void copy(ViewFrom&& from, ViewTo&& to)
	{
		zip(from, std::forward<ViewTo>(to),
			[](const auto& from, auto& to)
			{
				to = from;
			});
	}
	template <typename T, view ViewTo>
	constexpr void copy(const std::initializer_list<T>& l, ViewTo&& to)
	{
		copy(detail::wrap_initializer_list(l), std::forward<ViewTo>(to));
	}

	namespace functor
	{
		constexpr auto copy = []<typename ViewTo>(const auto& from, ViewTo&& to)
		{
			PPmatrix::copy(from, std::forward<ViewTo>(to));
		};
	}
}
