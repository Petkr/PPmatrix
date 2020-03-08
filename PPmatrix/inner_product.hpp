#pragma once
#include "zip.hpp"

namespace PPmatrix
{
	namespace detail
	{
		template <typename View1, typename View2>
		using accumulator_t = decltype(std::declval<view_base_t<View1>>() * std::declval<view_base_t<View2>>());
	}

	template <view View1, view View2,
		typename T = detail::accumulator_t<View1, View2>>
	constexpr T inner_product(const View1& view1, const View2& view2, T init = {})
	{
		zip(view1, view2,
			[&init](const auto& x, const auto& y)
			{
				init += x * y;
			});
		return init;
	}

	template <typename T, view View2,
		typename U = detail::accumulator_t<std::initializer_list<T>, View2>>
	constexpr U inner_product(const std::initializer_list<T>& l, const View2& view2, U init = {})
	{
		return inner_product(detail::wrap_initializer_list(l), view2, std::move(init));
	}

	template <view View1, typename T,
		typename U = detail::accumulator_t<View1, std::initializer_list<T>>>
	constexpr U inner_product(const View1& view1, const std::initializer_list<T>& l, U init = {})
	{
		return inner_product(view1, detail::wrap_initializer_list(l), std::move(init));
	}

	template <typename T, typename U,
		typename V = detail::accumulator_t<std::initializer_list<T>, std::initializer_list<U>>>
	constexpr V inner_product(const std::initializer_list<T>& l, const std::initializer_list<U>& m, V init = {})
	{
		return inner_product(detail::wrap_initializer_list(l), detail::wrap_initializer_list(m), std::move(init));
	}
}
