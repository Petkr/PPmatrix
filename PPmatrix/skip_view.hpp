#pragma once
#include <cstddef>
#include "simple_view.hpp"

namespace PPmatrix
{
	template <typename BaseIterator>
	class skip_iterator
	{
		BaseIterator base_iterator;
		std::size_t skip_length;
	public:
		constexpr skip_iterator(BaseIterator base_iterator, std::size_t skip_length)
			: base_iterator(base_iterator)
			, skip_length(skip_length)
		{}
		constexpr decltype(auto) operator*()
		{
			return *base_iterator;
		}
		constexpr auto& operator+=(std::size_t offset)
		{
			base_iterator += offset * skip_length;
			return *this;
		}
		constexpr auto& operator-=(std::size_t offset)
		{
			base_iterator -= offset * skip_length;
			return *this;
		}
		constexpr auto operator!=(BaseIterator other)
		{
			return base_iterator != other;
		}
		constexpr auto operator!=(skip_iterator other)
		{
			return base_iterator != other.base_iterator;
		}

		/*constexpr auto& base()
		{
			return base_iterator;
		}*/
	};

	struct skip
	{
		std::size_t skip_length;
		skip(std::size_t skip_length)
			: skip_length(skip_length)
		{}

		template <typename View>
		constexpr auto aligned_end(View&& view)
		{
			return end(view) + skip_length - PPmatrix::size(view) % skip_length;
		}
	};

	template <typename Iterator>
	constexpr auto operator&(Iterator i, skip s)
	{
		return skip_iterator(i, s.skip_length);
	}
	template <typename View>
	constexpr auto operator|(View&& view, skip s)
	{
		return begin(view) & s ^ s.aligned_end(std::forward<View>(view)) & s;
	}
	template <typename View>
	constexpr auto operator||(View&& view, skip s)
	{
		return begin(view) & s ^ s.aligned_end(std::forward<View>(view));
	}
}
