#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename View>
	constexpr auto max_element(const View& view)
	{
		auto i = begin(view);
		auto max = i;
		for (; i != end(view); ++i)
			if (*max < *i)
				max = i;
		return max;
	}
}
