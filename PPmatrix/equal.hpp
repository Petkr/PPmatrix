#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <view View1, view View2>
	constexpr bool equal(View1&& view1, View2&& view2)
	{
		auto i = begin(view1);
		auto j = begin(view2);
		for (; i != end(view1) && j != end(view2); ++i, ++j)
			if (*i != *j)
				return false;
		return true;
	}
	template <typename T, view View2>
	constexpr bool equal(const std::initializer_list<T>& l, View2&& view2)
	{
		return equal(detail::wrap_initializer_list(l), std::forward<View2>(view2));
	}
	template <view View1, typename T>
	constexpr bool equal(View1&& view1, const std::initializer_list<T>& l)
	{
		return equal(std::forward<View1>(view1), detail::wrap_initializer_list(l));
	}
	template <typename T, typename U>
	constexpr bool equal(const std::initializer_list<T>& l, const std::initializer_list<U>& m)
	{
		return equal(detail::wrap_initializer_list(l), detail::wrap_initializer_list(m));
	}
}
