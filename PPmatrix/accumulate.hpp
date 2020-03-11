#pragma once
#include <utility>
#include "view.hpp"
#include "next.hpp"

namespace PPmatrix
{
	template <view View, typename T = view_base_t<View>>
	constexpr T accumulate(View&& view, auto f, T init = {})
	{
		for (const auto& x : view)
			init = f(std::move(init), x);
		return init;
	}

	template <typename T, typename U = T>
	constexpr U accumulate(const std::initializer_list<T>& l, auto f, U init = {})
	{
		return accumulate(detail::wrap_initializer_list(l), std::move(f), std::move(init));
	}

	template <typename BinaryFunction>
	struct foldl
	{
		BinaryFunction f;
		constexpr foldl(BinaryFunction f)
			: f(f)
		{}
	};

	constexpr view auto operator|(view auto&& view, foldl<auto> f)
	{
		return accumulate(next(begin(view)) ^ end(view), std::move(f.f), *begin(view));
	}
}