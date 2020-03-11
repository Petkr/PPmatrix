#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr bool equal(view auto&& view1, view auto&& view2)
	{
		auto i = begin(view1);
		auto j = begin(view2);
		for (; i != end(view1) && j != end(view2); ++i, ++j)
			if (*i != *j)
				return false;
		return true;
	}
	constexpr bool equal(const std::initializer_list<auto>& l, view auto&& view2)
	{
		return equal(detail::wrap_initializer_list(l), view2);
	}
	constexpr bool equal(view auto&& view1, const std::initializer_list<auto>& l)
	{
		return equal(view1, detail::wrap_initializer_list(l));
	}
	constexpr bool equal(const std::initializer_list<auto>& l, const std::initializer_list<auto>& m)
	{
		return equal(detail::wrap_initializer_list(l), detail::wrap_initializer_list(m));
	}
}
