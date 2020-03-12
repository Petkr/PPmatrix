#pragma once
#include <cstddef>
#include "view.hpp"

namespace PPmatrix
{
	template <typename T, size_t size>
	class static_matrix
	{
		size_t width_;
		std::array<T, size> data_;
	public:

		constexpr explicit static_matrix(size_t width)
			: width_(width)
			, data_()
		{}

		constexpr iterator auto begin()
		{
			return PPmatrix::begin(data_);
		}
		constexpr iterator auto begin() const
		{
			return PPmatrix::begin(data_);
		}
		constexpr auto end()
		{
			return PPmatrix::end(data_);
		}
		constexpr auto end() const
		{
			return PPmatrix::end(data_);
		}
		constexpr size_t width() const
		{
			return width_;
		}
	};
}
