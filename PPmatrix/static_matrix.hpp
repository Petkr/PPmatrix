#pragma once
#include <cstddef>
#include "view.hpp"

namespace PPmatrix
{
	template <typename T, std::size_t size>
	class static_matrix
	{
		std::size_t width_;
		std::array<T, size> data_;
	public:

		constexpr explicit static_matrix(std::size_t width)
			: width_(width)
			, data_()
		{}

		constexpr auto begin()
		{
			return PPmatrix::begin(data_);
		}
		constexpr auto begin() const
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
		constexpr auto width() const
		{
			return width_;
		}
	};
}
