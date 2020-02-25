#pragma once

template <typename BiIterator>
constexpr auto prev(BiIterator i)
{
	return --i;
}
