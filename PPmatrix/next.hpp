#pragma once

namespace PPmatrix
{
	template <typename ForwardIterator>
	constexpr auto next(ForwardIterator i)
	{
		return ++i;
	}
}
