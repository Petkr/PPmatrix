#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename View, typename UnaryPredicate>
	constexpr auto find(const View& view, UnaryPredicate p)
	{
		auto i = begin(view);
		for (; i != end(view) && !p(*i); ++i);
		return i;
	}
}
