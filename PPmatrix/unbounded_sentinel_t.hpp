#pragma once
#include "view.hpp"

namespace PPmatrix
{
	struct unbounded_t {};
	inline constexpr unbounded_t unbounded{};

	consteval bool operator==(iterator auto, unbounded_t) { return false; }
	consteval void operator+=(unbounded_t, size_t) {}
	consteval void operator*(unbounded_t) {}
}
