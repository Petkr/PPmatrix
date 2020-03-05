#pragma once
#include <cstddef>
#include <vector>
#include "view.hpp"

namespace PPmatrix
{
	template <typename T, template <typename> typename container = std::vector>
	class dynamic_matrix
	{
		std::size_t width_;
		container<T> data_;

	public:
		dynamic_matrix(std::size_t height, std::size_t width)
			: width_(width)
			, data_(width_* height)
		{}

		auto begin()
		{
			return PPmatrix::begin(data_);
		}
		auto begin() const
		{
			return PPmatrix::begin(data_);
		}
		auto end()
		{
			return PPmatrix::end(data_);
		}
		auto end() const
		{
			return PPmatrix::end(data_);
		}
		auto width() const
		{
			return width_;
		}

		void resize(std::size_t size, std::size_t width)
		{
			data_.resize(size);
			width_ = width;
		}
	};
}
