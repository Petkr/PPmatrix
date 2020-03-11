#pragma once
#include <cstddef>
#include <vector>
#include "view.hpp"

namespace PPmatrix
{
	struct resize_columns_tag_t {};
	constexpr resize_columns_tag_t resize_columns_tag{};

	template <typename T, template <typename> typename container = std::vector>
	requires requires (container<T> c, std::size_t size)
	{
		requires view<container<T>>;
		container<T>(size);
		c.resize(size);
	}
	class dynamic_matrix
	{
		std::size_t width_;
		container<T> data_;

	public:
		dynamic_matrix(std::size_t height, std::size_t width)
			: width_(width)
			, data_(width_* height)
		{}

		iterator auto begin()
		{
			return PPmatrix::begin(data_);
		}
		iterator auto begin() const
		{
			return PPmatrix::begin(data_);
		}
		iterator auto end()
		{
			return PPmatrix::end(data_);
		}
		iterator auto end() const
		{
			return PPmatrix::end(data_);
		}
		std::size_t width() const
		{
			return width_;
		}
		void resize(std::size_t new_height)
		{
			data_.resize(width_ * new_height);
		}
		void resize(std::size_t new_width, resize_columns_tag_t)
		{
			auto old_height = height(*this);
			width_ = new_width;
			data_.resize(width_ * old_height);
		}
	};
}
