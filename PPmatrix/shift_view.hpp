#pragma once
#include "view.hpp"

namespace PPmatrix
{
	struct shift
	{
		size_t count;
		shift(size_t count)
			: count(count)
		{}
	};

	constexpr view auto operator<<(view auto&& v, shift s)
	{
		return begin(v) - s.count ^ end(v) - s.count;
	}
	constexpr view auto operator<<(shift s, view auto&& v)
	{
		return v << s;
	}
	constexpr view auto operator<(view auto&& v, shift s)
	{
		return begin(v) ^ end(v) - s.count;
	}
	constexpr view auto operator<(shift s, view auto&& v)
	{
		return begin(v) - s.count ^ end(v);
	}
	constexpr view auto operator>(view auto&& v, shift s)
	{
		return begin(v) ^ end(v) + s.count;
	}
	constexpr view auto operator>(shift s, view auto&& v)
	{
		return begin(v) + s.count ^ end(v);
	}
}
