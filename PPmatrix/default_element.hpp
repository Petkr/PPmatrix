#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr auto default_element(view auto&& v)
	{
		return view_base_t<decltype(v)>();
	}
}
