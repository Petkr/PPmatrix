#pragma once
#include "iterator.hpp"

namespace PPmatrix
{
	template <iterator BiIterator>
	constexpr /*same<*/BiIterator/*> auto*/ prev(BiIterator i)
	{
		return --i;
	}
}
