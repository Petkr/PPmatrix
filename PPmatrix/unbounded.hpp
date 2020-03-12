#pragma once
#include "iterator.hpp"

namespace PPmatrix
{
	struct unbounded_t {};
	inline constexpr unbounded_t unbounded{};

	constexpr bool operator==(iterator auto, unbounded_t) { return false; }
}
