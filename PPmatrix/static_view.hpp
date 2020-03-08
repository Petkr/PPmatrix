#pragma once
#include <cstddef>
#include "view.hpp"

namespace PPmatrix
{
	template <typename T>
	class static_iterator
	{
		T value;
	public:
		constexpr static_iterator(T&& value)
			: value(std::move(value))
		{}
		constexpr static_iterator(const T& value)
			: value(value)
		{}
		constexpr auto& operator*()
		{
			return value;
		}
		constexpr auto& operator+=(std::size_t)
		{
			return *this;
		}
		constexpr auto& operator-=(std::size_t)
		{
			return *this;
		}
	};

	template <typename T>
	constexpr view static_view(T&& value)
	{
		return static_iterator(std::forward<T>(value)) ^ unbounded;
	}
}
