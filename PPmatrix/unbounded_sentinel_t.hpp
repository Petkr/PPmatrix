#pragma once
#include "view.hpp"

namespace PPmatrix
{
	template <typename Iterator>
	constexpr auto iterator_has_value = !std::is_same_v<void, iterator_base_t<Iterator>>;

	struct unbounded_t {};
	constexpr unbounded_t unbounded{};

	template <typename Iterator>
	constexpr auto operator!=(Iterator, unbounded_t) { return true; }
	template <typename Iterator>
	constexpr auto operator!=(unbounded_t, Iterator) { return true; }
	constexpr void operator+=(unbounded_t, std::size_t) {}
	constexpr auto operator*(unbounded_t) {}

	template <typename Iterator>
	constexpr auto is_unbounded_iterator = std::is_same_v<unbounded_t, iterator_base_t<Iterator>>;
}
