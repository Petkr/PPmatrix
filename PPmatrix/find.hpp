#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename View, typename UnaryPredicate>
	constexpr auto find(View&& view, UnaryPredicate p)
	{
		auto i = begin(view);
		for (; i != end(view) && !p(*i); ++i);
		return i;
	}

	template <typename T, typename UnaryPredicate>
	constexpr auto find(const std::initializer_list<T>& l, UnaryPredicate p)
	{
		return find(detail::wrap_initializer_list(l), std::move(p));
	}
}
