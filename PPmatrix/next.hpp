#pragma once

template <typename ForwardIterator>
constexpr auto next(ForwardIterator i)
{
	return ++i;
}
