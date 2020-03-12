#pragma once
#include <cstddef>
#include <vector>
#include "view.hpp"

namespace PPmatrix
{
	struct resize_columns_tag_t {};
	constexpr resize_columns_tag_t resize_columns_tag{};

	template <typename T, template <typename> typename container = std::vector>
	requires requires (container<T> c, size_t size)
	{
		requires view<container<T>>;
		container<T>(size);
		c.resize(size);
	}
	class dynamic_matrix
	{
		size_t width_;
		container<T> data_;

	public:
		dynamic_matrix(size_t height, size_t width)
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
		auto end()
		{
			return PPmatrix::end(data_);
		}
		auto end() const
		{
			return PPmatrix::end(data_);
		}
		size_t width() const
		{
			return width_;
		}
		void flip_dimensions()
		{
			width_ = height(*this);
		}
		void resize(size_t new_height)
		{
			data_.resize(width_ * new_height);
		}
		void resize(size_t new_width, resize_columns_tag_t)
		{
			auto old_height = height(*this);
			width_ = new_width;
			data_.resize(width_ * old_height);
		}
	};
}
