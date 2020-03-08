#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <view View>
	constexpr iterator max_element(View&& view)
	{
		auto i = begin(view);
		auto max = i;
		for (; i != end(view); ++i)
			if (*max < *i)
				max = i;
		return max;
	}
	template <typename T>
	constexpr iterator max_element(const std::initializer_list<T>& l)
	{
		return max_element(detail::wrap_initializer_list(l));
	}
}
