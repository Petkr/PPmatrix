#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr iterator auto find(view auto&& view, auto p)
	{
		auto i = begin(view);
		for (; i != end(view) && !p(*i); ++i);
		return i;
	}

	constexpr iterator auto find(const std::initializer_list<auto>& l, auto p)
	{
		return find(detail::wrap_initializer_list(l), std::move(p));
	}
}
