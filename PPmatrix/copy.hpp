#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	template <typename ViewFrom, typename ViewTo>
	constexpr void copy(const ViewFrom& from, ViewTo&& to)
	{
		zip(from, std::forward<ViewTo>(to),
			[](const auto& from, auto& to)
			{
				to = from;
			});
	}
	template <typename T, typename ViewTo>
	constexpr void copy(const std::initializer_list<T>& from, ViewTo&& to)
	{
		copy(detail::wrap_initializer_list(from), std::forward<ViewTo>(to));
	}

	namespace functor
	{
		constexpr auto copy = []<typename ViewTo>(const auto& from, ViewTo&& to)
		{
			copy(from, std::forward<ViewTo>(to));
		};
	}
}
