#pragma once
#include "iterator.hpp"

namespace PPmatrix
{
	template <iterator ForwardIterator>
	constexpr /*same<*/ForwardIterator/*> auto*/ next(ForwardIterator i)
	{
		return ++i;
	}
}
