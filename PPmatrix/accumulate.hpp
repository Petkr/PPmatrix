#pragma once
#include <utility>
#include "view.hpp"
#include "next.hpp"

namespace PPmatrix
{
	template <typename View, typename BinaryFunction,
		typename T = view_base_t<View>>
	constexpr auto accumulate(const View& view, BinaryFunction f, T init = {})
	{
		for (const auto& x : view)
			init = f(std::move(init), x);
		return init;
	}

	template <typename T, typename BinaryFunction,
		typename U = view_base_t<std::initializer_list<T>>>
	constexpr auto accumulate(const std::initializer_list<T>& l, BinaryFunction f, U init = {})
	{
		return accumulate(detail::wrap_initializer_list(l), f, std::move(init));
	}

	template <typename BinaryFunction>
	struct foldl
	{
		BinaryFunction f;
		constexpr foldl(BinaryFunction f)
			: f(f)
		{}
	};

	template <typename View, typename BinaryFunction>
	constexpr auto operator|(View&& view, foldl<BinaryFunction> f)
	{
		return accumulate(next(begin(view)) ^ end(view), f.f, *begin(view));
	}
}