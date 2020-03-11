#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr iterator auto max_element(view auto&& v)
	{
		auto i = begin(v);
		auto max = i;
		for (; i != end(v); ++i)
			if (*max < *i)
				max = i;
		return max;
	}
	constexpr iterator auto max_element(const std::initializer_list<auto>& l)
	{
		return max_element(detail::wrap_initializer_list(l));
	}
}
