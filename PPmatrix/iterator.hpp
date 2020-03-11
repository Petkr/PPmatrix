#pragma once
#include "same.hpp"
#include "nonvoid.hpp"
#include "size_t.hpp"

namespace PPmatrix
{
	namespace detail
	{
		template <typename T>
		concept has_operator_advance = requires (T t, size_t n)
		{
			t += n;
		};
		template <typename T>
		concept has_operator_back = requires (T t, size_t n)
		{
			t -= n;
		};
	}

	constexpr auto operator+(detail::has_operator_advance auto t, size_t u)
	{
		t += u;
		return t;
	}
	constexpr auto operator-(detail::has_operator_back auto t, size_t u)
	{
		t -= u;
		return t;
	}
	constexpr auto& operator++(detail::has_operator_advance auto& t)
	{
		t += 1;
		return t;
	}
	constexpr auto& operator--(detail::has_operator_back auto& t)
	{
		t -= 1;
		return t;
	}

	template <typename Iterator>
	concept iterator = requires (Iterator i)
	{
		{ ++i } -> same<Iterator&>;
		{ *i } -> nonvoid;
	};

	template <typename Sentinel, typename Iterator>
	concept sentinel =
		iterator<Iterator> &&
		requires (const Iterator i, const Sentinel s)
		{
			{ i == s } -> same<bool>;
		};
}
