#pragma once
#include "view.hpp"
#include "unbounded_sentinel_t.hpp"

namespace PPmatrix
{
	template <typename T>
	class wrap_iterator
	{
		T value;
	public:
		// can't use forwarding reference
		// messes with implicit copy/move constructors
		constexpr wrap_iterator(const T& value)
			: value(value)
		{}
		constexpr wrap_iterator(T&& value)
			: value(std::move(value))
		{}
		constexpr auto& operator*()
		{
			return value;
		}
		constexpr const auto& operator*() const
		{
			return value;
		}
		constexpr auto& operator+=(std::size_t offset)
		{
			value += offset;
			return *this;
		}
		constexpr auto operator==(const T& other) const
		{
			return value == other;
		}
		constexpr auto operator==(const wrap_iterator<auto>& other) const
		{
			return value == *other;
		}
	};

	template <typename T, typename U>
	constexpr view wrap_view(T&& begin, U&& end)
	{
		return wrap_iterator(std::forward<T>(begin)) ^ wrap_iterator(std::forward<U>(end));
	}

	template <typename T>
	constexpr view wrap_view(T&& begin)
	{
		return wrap_iterator(std::forward<T>(begin)) ^ unbounded_sentinel;
	}
}
