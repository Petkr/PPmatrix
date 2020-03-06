#pragma once
#include "view.hpp"

namespace PPmatrix
{
	struct unbounded_t {};
	constexpr unbounded_t unbounded{};

	template <iterator Iterator>
	requires !requires (Iterator i) { i != unbounded; }
	constexpr auto operator!=(Iterator, unbounded_t) { return true; }
	constexpr void operator+=(unbounded_t, std::size_t) {}
	constexpr auto operator*(unbounded_t) {}
}
