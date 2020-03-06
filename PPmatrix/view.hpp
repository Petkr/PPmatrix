#pragma once
#include <cstddef>
#include <utility>
#include <type_traits>
#include "iterator.hpp"

namespace PPmatrix
{
	template <typename View>
	requires requires (View v) { v.begin(); }
	constexpr auto begin(View&& view)
	{
		return view.begin();
	}

	template <typename View>
	requires requires (View v) { v.end(); }
	constexpr auto end(View&& view)
	{
		return view.end();
	}

	template <typename View>
	concept view = requires (View v)
	{
		begin(v);
		end(v);
	};

	namespace detail
	{
		template <typename T>
		concept has_size = requires (const T t)
		{
			t.size();
		};
	}
	template <view View>
	constexpr std::size_t size(View&& view)
	{
		if constexpr (detail::has_size<View>)
			return view.size();
		else
			return end(view) - begin(view);
	}

	template <typename View>
	using begin_t = decltype(begin(std::declval<View>()));
	template <typename View>
	using end_t = decltype(end(std::declval<View>()));
	template <typename Iterator>
	using iterator_base_t = std::decay_t<decltype(*std::declval<Iterator>())>;
	template <typename View>
	using view_base_t = iterator_base_t<begin_t<View>>;

	namespace detail
	{
		template <typename T>
		struct initializer_list_wrapper
		{
			std::initializer_list<T> x;

			constexpr auto begin() const
			{
				return x.begin();
			}
			constexpr auto end() const
			{
				return x.end();
			}
			constexpr auto size() const
			{
				return x.size();
			}
		};
		template <typename T>
		constexpr auto& wrap_initializer_list(const std::initializer_list<T>& l)
		{
			return reinterpret_cast<const initializer_list_wrapper<T>&>(l);
		}
	}
}
