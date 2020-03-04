#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	template <typename View1, typename View2,
		typename T = decltype(std::declval<view_base_t<View1>>()* std::declval<view_base_t<View2>>())>
		constexpr auto inner_product(const View1& view1, const View2& view2, T init = {})
	{
		zip(view1, view2,
			[&init](auto&& x, auto&& y)
			{
				init += x * y;
			});
		return init;
	}
}
