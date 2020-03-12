#pragma once
#include <type_traits>

namespace PPmatrix
{
	struct empty {};

	namespace detail
	{
		// saves memory when one type from the pair is empty
		//
		// useful in simple_view with unbounded, where the only
		// information is the type of its end iterator, not data
		//
		// for the same reason also in transform_iterator,
		// if the transform is a captureless lambda or other empty functor

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
			// **might be redundant**
			// in case of non-trivial special member functions in the union
			constexpr compressed_pair_empty_first(const compressed_pair_empty_first& other)
				: second(other.second)
			{}
			constexpr auto& operator=(const compressed_pair_empty_first& other)
			{
				second = other.second;
				return *this;
			}
			~compressed_pair_empty_first()
			{
				second.~Second();
			}
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
			// **might be redundant**
			// in case of non-trivial special member functions in the union
			constexpr compressed_pair_empty_second(const compressed_pair_empty_second& other)
				: first(other.first)
			{}
			constexpr auto& operator=(const compressed_pair_empty_second& other)
			{
				first = other.first;
				return *this;
			}
			~compressed_pair_empty_second()
			{
				first.~First();
			}
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
}
