#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename View>
	constexpr auto max_element(View&& view)
	{
		auto i = begin(view);
		auto max = i;
		for (; i != end(view); ++i)
			if (*max < *i)
				max = i;
		return max;
	}
	template <typename T>
	constexpr auto max_element(const std::initializer_list<T>& l)
	{
		return max_element(detail::wrap_initializer_list(l));
	}
}
