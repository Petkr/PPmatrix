#pragma once

namespace PPmatrix
{
	namespace detail
	{
		template <typename T>
		concept has_operator_advance = requires (T t, std::size_t n)
		{
			t += n;
		};
		template <typename T>
		concept has_operator_back = requires (T t, std::size_t n)
		{
			t -= n;
		};
	}

	template <detail::has_operator_advance T>
	constexpr auto operator+(T t, std::size_t u)
	{
		t += u;
		return t;
	}
	template <detail::has_operator_back T>
	constexpr auto operator-(T t, std::size_t u)
	{
		t -= u;
		return t;
	}
	template <detail::has_operator_advance T>
	constexpr auto& operator++(T& t)
	{
		t += 1;
		return t;
	}
	template <detail::has_operator_back T>
	constexpr auto& operator--(T& t)
	{
		t -= 1;
		return t;
	}

	template <typename Sentinel, typename Iterator>
	concept sentinel = requires (Iterator i, Sentinel s)
	{
		i != s;
	};

	template <typename Iterator1, typename Iterator2>
	requires sentinel<Iterator2, Iterator1> || sentinel<Iterator1, Iterator2>
	constexpr auto compare_iterator(Iterator1 i1, Iterator2 i2)
	{
		if constexpr (sentinel<Iterator2, Iterator1>)
			return i1 != i2;
		else
			return i2 != i1;
	}


	template <typename Iterator>
	concept iterator =
		//sentinel<Iterator, Iterator> &&
		requires (Iterator i)
		{
			++i;
			*i;
		};
}
