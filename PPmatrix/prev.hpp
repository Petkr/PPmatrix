#pragma once

namespace PPmatrix
{
	template <typename BiIterator>
	constexpr auto prev(BiIterator i)
	{
		return --i;
	}
}
