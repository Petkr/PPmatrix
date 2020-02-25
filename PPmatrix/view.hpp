#pragma once
#include <cstddef>
#include <utility>
#include <type_traits>
#include "operators.hpp"

template <typename View>
concept has_begin = requires (View v)
{
	v.begin();
};
template <typename View>
requires has_begin<View>
constexpr auto begin(View&& view)
{
	return view.begin();
}

template <typename View>
concept has_end = requires (View v)
{
	v.end();
};
template <typename View>
requires has_end<View>
constexpr auto end(View&& view)
{
	return view.end();
}

template <typename View>
concept is_view = requires (View v)
{
	begin(v);
	end(v);
};

template <typename T>
concept has_size = requires (const T t)
{
	{ t.size() } -> std::size_t;
};
template <typename View>
constexpr std::size_t size(const View& view)
{
	if constexpr (has_size<View>)
		return view.size();
	else
		return end(view) - begin(view);
}

template <typename View>
using begin_t = decltype(begin(std::declval<View>()));
template <typename View>
using end_t = decltype(end(std::declval<View>()));
template <typename Iterator>
using iterator_base_t = std::decay_t<decltype(*std::declval<Iterator>())>;
template <typename View>
using view_base_t = iterator_base_t<begin_t<View>>;
