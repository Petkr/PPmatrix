#pragma once
#include "zip.hpp"

template <typename ViewFrom, typename ViewTo>
constexpr void copy(const ViewFrom& from, ViewTo&& to)
{
	zip(from, std::forward<ViewTo>(to),
		[](const auto& from, auto& to)
		{
			to = from;
		});
}

namespace functor
{
	constexpr auto copy = [](auto&& x, auto&& y) { copy(x, y); };
}
