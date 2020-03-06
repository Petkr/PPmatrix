#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename Function, view ViewV, view ViewW>
	constexpr void zip(ViewV&& v, ViewW&& w, Function f)
	{
		auto i = begin(v);
		auto j = begin(w);
		for (; i != end(v) && j != end(w); ++i, ++j)
			f(*i, *j);
	}
	template <typename Function, typename T, view ViewW>
	constexpr void zip(const std::initializer_list<T>& l, ViewW&& w, Function f)
	{
		zip(detail::wrap_initializer_list(l), std::forward<ViewW>(w), f);
	}
	template <typename Function, view ViewV, typename T>
	constexpr void zip(ViewV&& v, const std::initializer_list<T>& l, Function f)
	{
		zip(std::forward<ViewV>(v), detail::wrap_initializer_list(l), f);
	}
	template <typename Function, typename T, typename U>
	constexpr void zip(const std::initializer_list<T>& l, const std::initializer_list<U>& m, Function f)
	{
		zip(detail::wrap_initializer_list(l), detail::wrap_initializer_list(m), f);
	}
}