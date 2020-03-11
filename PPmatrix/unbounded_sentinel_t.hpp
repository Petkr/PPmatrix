#pragma once
#include "view.hpp"

namespace PPmatrix
{
	struct unbounded_t {};
	constexpr unbounded_t unbounded{};

	consteval bool operator==(iterator auto, unbounded_t) { return false; }
	consteval void operator+=(unbounded_t, std::size_t) {}
	consteval void operator*(unbounded_t) {}
}
