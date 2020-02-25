#pragma once
#include "view.hpp"

struct shift_begin_left
{
	std::size_t count;
	shift_begin_left(std::size_t count)
		: count(count)
	{}
};
struct shift_begin_right
{
	std::size_t count;
	shift_begin_right(std::size_t count)
		: count(count)
	{}
};
struct shift_end_left
{
	std::size_t count;
	shift_end_left(std::size_t count)
		: count(count)
	{}
};
struct shift_end_right
{
	std::size_t count;
	shift_end_right(std::size_t count)
		: count(count)
	{}
};
struct shift_left
{
	std::size_t count;
	shift_left(std::size_t count)
		: count(count)
	{}
};
struct shift_right
{
	std::size_t count;
	shift_right(std::size_t count)
		: count(count)
	{}
};

template <typename View>
constexpr auto operator|(View&& view, shift_begin_left s)
{
	return begin(view) - s.count ^ end(view);
}
template <typename View>
constexpr auto operator|(View&& view, shift_begin_right s)
{
	return begin(view) + s.count ^ end(view);
}
template <typename View>
constexpr auto operator|(View&& view, shift_end_left s)
{
	return simple_view(begin(view), end(view) - s.count);
}
template <typename View>
constexpr auto operator|(View&& view, shift_end_right s)
{
	return begin(view) ^ end(view) + s.count;
}
template <typename View>
constexpr auto operator|(View&& view, shift_left s)
{
	return begin(view) - s.count ^ end(view) - s.count;
}
template <typename View>
constexpr auto operator|(View&& view, shift_right s)
{
	return begin(view) + s.count ^ end(view) + s.count;
}
