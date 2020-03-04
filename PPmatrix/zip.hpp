#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename Function, typename ViewV, typename ViewW>
	constexpr void zip(ViewV&& v, ViewW&& w, Function f)
	{
		auto i = begin(v);
		auto j = begin(w);
		for (; i != end(v) && j != end(w); ++i, ++j)
			f(*i, *j);
	}

	template <typename Function, typename ViewV, typename ViewW, typename ViewZ>
	constexpr void zip(ViewV&& v, ViewW&& w, ViewZ&& z, Function f)
	{
		auto i = begin(v);
		auto j = begin(w);
		auto k = begin(z);
		for (; i != end(v) && j != end(w) && k != end(z); ++i, ++j, ++k)
			f(*i, *j, *k);
	}
}