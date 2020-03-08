#pragma once
#include "iterator.hpp"

namespace PPmatrix
{
	template <iterator ForwardIterator>
	constexpr ForwardIterator next(ForwardIterator i)
	{
		return ++i;
	}
}
