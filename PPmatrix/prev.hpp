#pragma once
#include "iterator.hpp"

namespace PPmatrix
{
	template <iterator BiIterator>
	constexpr BiIterator prev(BiIterator i)
	{
		return --i;
	}
}
