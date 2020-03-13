#pragma once
#include <cstddef>
#include <vector>
#include "view.hpp"
#include "PP.hpp"

namespace PPmatrix
{
	struct change_width_tag_t {};
	constexpr change_width_tag_t change_width_tag{};

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
			return PP::begin(data_);
		}
		iterator auto begin() const
		{
			return PP::begin(data_);
		}
		auto end()
		{
			return PP::end(data_);
		}
		auto end() const
		{
			return PP::end(data_);
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
		void resize(size_t new_width, change_width_tag_t)
		{
			auto old_height = height(*this);
			width_ = new_width;
			data_.resize(width_ * old_height);
		}
	};
}
