#pragma once
#include "utility.hpp"

template <typename View1, typename View2>
constexpr auto equal(View1&& view1, View2&& view2)
{
	auto i = begin(view1);
	auto j = begin(view2);
	for (; i != end(view1) && j != end(view2); ++i, ++j)
		if (*i != *j)
			return false;
	return true;
}
