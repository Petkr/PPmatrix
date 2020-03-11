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
		constexpr const auto& operator*() const
		{
			return value;
		}
		constexpr auto& operator+=(size_t)
		{
			return *this;
		}
		constexpr auto& operator-=(size_t)
		{
			return *this;
		}
		constexpr auto operator==(const static_iterator<auto>& other) const
		{
			return value == other.value;
		}
	};

	template <typename T>
	constexpr view auto static_view(T&& value)
	{
		return static_iterator(std::forward<T>(value)) ^ unbounded;
	}
}
