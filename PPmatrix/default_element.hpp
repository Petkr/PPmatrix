#pragma once
#include "view.hpp"

namespace PPmatrix
{
	// in an abbreviated template function, which takes a view
	// the type of the view is not avaiable
	//
	// instead of needing to use full template syntax
	// for the type name or using decltype(my_view)
	//
	// one can use
	// auto x = default_element(my_view);

	constexpr auto default_element(view auto&& v)
	{
		return view_base_t<decltype(v)>();
	}
}
