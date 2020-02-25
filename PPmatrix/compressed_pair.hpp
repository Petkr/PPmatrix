#pragma once
#include <type_traits>

struct empty {};

namespace detail
{
	template <typename First, typename Second>
	struct compressed_pair_nonempty
	{
		First first;
		Second second;

		constexpr compressed_pair_nonempty(First first, Second second)
			: first(first)
			, second(second)
		{}
	};
	template <typename Empty, typename Second>
	struct compressed_pair_empty_first
	{
		union
		{
			Second second;
			Empty first;
		};

		constexpr compressed_pair_empty_first(Empty, Second second)
			: second(second)
		{}
		/*constexpr compressed_pair_empty_first(const compressed_pair_empty_first& other)
			: second(other.second)
		{}
		~compressed_pair_empty_first()
		{}
		constexpr auto& operator=(const compressed_pair_empty_first& other)
		{
			second = other.second;
			return *this;
		}*/
	};
	template <typename First, typename Empty>
	struct compressed_pair_empty_second
	{
		union
		{
			First first;
			Empty second;
		};

		constexpr compressed_pair_empty_second(First first, Empty)
			: first(first)
		{}
		/*constexpr compressed_pair_empty_second(const compressed_pair_empty_second& other)
			: first(other.first)
		{}
		~compressed_pair_empty_second()
		{}
		constexpr auto& operator=(const compressed_pair_empty_second& other)
		{
			first = other.first;
			return *this;
		}*/
	};

	template <typename First, typename Second>
	constexpr auto compressed_pair_dispatch()
	{
		if constexpr (std::is_empty_v<First>)
			return std::type_identity<compressed_pair_empty_first<First, Second>>{};
		else if constexpr (std::is_empty_v<Second>)
			return std::type_identity<compressed_pair_empty_second<First, Second>>{};
		else
			return std::type_identity<compressed_pair_nonempty<First, Second>>{};
	}
}

template <typename First, typename Second>
using compressed_pair = typename decltype(detail::compressed_pair_dispatch<First, Second>())::type;

template <typename T>
constexpr auto is_compressed_pair = false;
template <typename T, typename U>
constexpr auto is_compressed_pair<detail::compressed_pair_empty_first<T, U>> = true;
template <typename T, typename U>
constexpr auto is_compressed_pair<detail::compressed_pair_empty_second<T, U>> = true;
template <typename T, typename U>
constexpr auto is_compressed_pair<detail::compressed_pair_nonempty<T, U>> = true;

template <typename First, typename Second>
constexpr auto make_compressed_pair(First first, Second second)
{
	return compressed_pair<First, Second>(first, second);
}

template <typename F, typename Pair>
constexpr auto apply(F f, Pair&& pair)
{
	if constexpr (is_compressed_pair<std::decay_t<Pair>>)
		return f(pair.first, pair.second);
	else
		return f(std::forward<Pair>(pair));
}

template <typename F, typename Pair>
constexpr auto for_each_pair_element(F f, Pair&& pair)
{
	if constexpr (	!std::is_void_v<std::invoke_result_t<F, decltype(pair.first )>> && 
					!std::is_void_v<std::invoke_result_t<F, decltype(pair.second)>>)
	{
		return make_compressed_pair(f(pair.first), f(pair.second));
	}
	else
	{
		f(pair.first);
		f(pair.second);
	}
}
