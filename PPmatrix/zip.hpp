#pragma once
#include "view.hpp"

namespace PPmatrix
{
	constexpr void zip(view auto&& v, view auto&& w, auto f)
	{
		auto i = begin(v);
		auto j = begin(w);
		for (; i != end(v) && j != end(w); ++i, ++j)
			f(*i, *j);
	}
	constexpr void zip(const std::initializer_list<auto>& l, view auto&& w, auto f)
	{
		zip(detail::wrap_initializer_list(l), w, std::move(f));
	}
	constexpr void zip(view auto&& v, const std::initializer_list<auto>& l, auto f)
	{
		zip(v, detail::wrap_initializer_list(l), std::move(f));
	}
	constexpr void zip(const std::initializer_list<auto>& l, const std::initializer_list<auto>& m, auto f)
	{
		zip(detail::wrap_initializer_list(l), detail::wrap_initializer_list(m), std::move(f));
	}
}
