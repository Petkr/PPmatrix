#pragma once
#include "simple_matrix_view.hpp"
#include "matrix_view.hpp"
#include "prev.hpp"

namespace PPmatrix
{
	template <iterator LeftIterator, iterator RightIterator>
	class augmented_matrix_view_iterator
	{
		size_t left_pos;
		size_t left_width;
		LeftIterator left_iterator;
		size_t right_pos;
		size_t right_width;
		RightIterator right_iterator;
		bool left_active;

		void move_left(size_t offset)
		{
			left_iterator += offset;
			left_pos += offset;
		}
		void move_right(size_t offset)
		{
			right_iterator += offset;
			right_pos += offset;
		}
		void up_rows(size_t rows)
		{
			left_iterator -= rows * left_width;
			right_iterator -= rows * right_width;
		}
		void down_rows(size_t rows)
		{
			left_iterator += rows * left_width;
			right_iterator += rows * right_width;
		}

		template <iterator, iterator>
		friend class augmented_matrix_view_iterator;

	public:
		augmented_matrix_view_iterator(LeftIterator left_iterator, size_t left_width, RightIterator right_iterator, size_t right_width)
			: left_pos(0)
			, left_width(left_width)
			, left_iterator(left_iterator)
			, right_pos(0)
			, right_width(right_width)
			, right_iterator(right_iterator)
			, left_active(true)
		{}

		constexpr decltype(auto) operator*() const
		{
			if (left_active)
				return *left_iterator;
			else
				return *right_iterator;
		}
		constexpr auto& operator++()
		{
			if (left_active)
			{
				++left_pos;
				++left_iterator;
				if (left_pos == left_width)
				{
					left_pos = 0;
					left_active = false;
				}
			}
			else
			{
				++right_pos;
				++right_iterator;
				if (right_pos == right_width)
				{
					right_pos = 0;
					left_active = true;
				}
			}

			return *this;
		}
		constexpr auto& operator--()
		{
			if (left_active)
			{
				--left_iterator;
				--left_pos;
				if (left_pos == size_t(-1))
				{
					right_iterator -= right_width;
					left_pos = left_width - 1;
					left_active = false;
				}
			}
			else
			{
				--right_iterator;
				--right_pos;
				if (right_pos == size_t(-1))
				{
					right_pos = right_width - 1;
					left_active = true;
				}
			}

			return *this;
		}
		constexpr auto& operator+=(size_t offset)
		{
			auto w = left_width + right_width;
			auto rows = offset / w;
			auto cols = offset % w;

			down_rows(rows);

			for (size_t i = 0; i != cols; ++i)
				++(*this);

			return *this;
		}
		constexpr auto& operator-=(size_t offset)
		{
			auto w = left_width + right_width;
			auto rows = offset / w;
			auto cols = offset % w;

			up_rows(rows);

			for (size_t i = 0; i != cols; ++i)
				--(*this);

			return *this;
		}
		template <iterator LeftIteratorOther, iterator RightIteratorOther>
		constexpr bool operator==(const augmented_matrix_view_iterator<LeftIteratorOther, RightIteratorOther>& other) const
		{
			if (left_active == other.left_active)
			{
				if (left_active)
					return left_iterator == other.left_iterator;
				else
					return right_iterator == other.right_iterator;
			}
			return false;
		}
		template <iterator Iterator>
		constexpr bool operator==(Iterator other) const
		{
			if (left_active)
			{
				if constexpr (sentinel<Iterator, LeftIterator>)
					return left_iterator == other;
			}
			else
			{
				if constexpr (sentinel<Iterator, RightIterator>)
					return right_iterator == other;
			}
			return false; //shouldn't happen
		}
	};

	struct dont_check_heights_tag_t {};
	constexpr dont_check_heights_tag_t dont_check_heights_tag{};

	template <matrix_view LeftMatrixView, matrix_view RightMatrixView>
	class augmented_matrix_view
	{
		LeftMatrixView left;
		RightMatrixView right;

	public:
		augmented_matrix_view(matrix_view auto&& left, matrix_view auto&& right)
			: augmented_matrix_view(left, right, dont_check_heights_tag)
		{
			if (height(left) != height(right))
				throw "incompatible heights";
		}
		augmented_matrix_view(matrix_view auto&& left, matrix_view auto&& right, dont_check_heights_tag_t)
			: left(simple_matrix_view(left))
			, right(simple_matrix_view(right))
		{}

		iterator auto begin() const
		{
			return augmented_matrix_view_iterator(
				PPmatrix::begin(left),
				PPmatrix::width(left),
				PPmatrix::begin(right),
				PPmatrix::width(right));
		}
		iterator auto end() const
		{
			return augmented_matrix_view_iterator(
				PPmatrix::end(left),
				PPmatrix::width(left),
				PPmatrix::end(right),
				PPmatrix::width(right));
		}
		size_t width() const
		{
			return PPmatrix::width(left) + PPmatrix::width(right);
		}
		size_t size() const
		{
			return PPmatrix::size(left) + PPmatrix::size(right);
		}
	};
	augmented_matrix_view(matrix_view auto&& left, matrix_view auto&& right)
		-> augmented_matrix_view<
		decltype(simple_matrix_view(left)),
		decltype(simple_matrix_view(right))>;
}
