#pragma once
#include "simple_matrix_view.hpp"
#include "matrix_view.hpp"
#include "prev.hpp"

namespace PPmatrix
{
	template <typename LeftIterator, typename RightIterator>
	class augmented_matrix_view_iterator
	{
		std::size_t left_pos;
		std::size_t left_width;
		LeftIterator left_iterator;
		std::size_t right_pos;
		std::size_t right_width;
		RightIterator right_iterator;
		bool left_active;

		void move_left(std::size_t offset)
		{
			left_iterator += offset;
			left_pos += offset;
		}
		void move_right(std::size_t offset)
		{
			right_iterator += offset;
			right_pos += offset;
		}
		void up_rows(std::size_t rows)
		{
			left_iterator -= rows * left_width;
			right_iterator -= rows * right_width;
		}
		void down_rows(std::size_t rows)
		{
			left_iterator += rows * left_width;
			right_iterator += rows * right_width;
		}

		template <typename, typename>
		friend class augmented_matrix_view_iterator;

	public:
		augmented_matrix_view_iterator(LeftIterator left_iterator, std::size_t left_width, RightIterator right_iterator, std::size_t right_width)
			: left_pos(0)
			, left_width(left_width)
			, left_iterator(left_iterator)
			, right_pos(0)
			, right_width(right_width)
			, right_iterator(right_iterator)
			, left_active(true)
		{}

		constexpr auto& operator*()
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
				if (left_pos == std::size_t(-1))
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
				if (right_pos == std::size_t(-1))
				{
					right_pos = right_width - 1;
					left_active = true;
				}
			}

			return *this;
		}
		constexpr auto& operator+=(std::size_t offset)
		{
			auto w = left_width + right_width;
			auto rows = offset / w;
			auto cols = offset % w;

			down_rows(rows);

			for (std::size_t i = 0; i != cols; ++i)
				++(*this);

			return *this;
		}
		constexpr auto& operator-=(std::size_t offset)
		{
			auto w = left_width + right_width;
			auto rows = offset / w;
			auto cols = offset % w;

			up_rows(rows);

			for (std::size_t i = 0; i != cols; ++i)
				--(*this);

			return *this;
		}
		template <iterator LeftIteratorOther, iterator RightIteratorOther>
		constexpr auto operator!=(const augmented_matrix_view_iterator<LeftIteratorOther, RightIteratorOther>& other) const
		{
			if (left_active == other.left_active)
			{
				if (left_active)
					return compare_iterator(left_iterator, other.left_iterator);
				else
					return compare_iterator(right_iterator, other.right_iterator);
			}
			return true;
		}
		template <iterator Iterator>
		constexpr auto operator!=(Iterator other) const
		{
			if (left_active)
			{
				if constexpr (sentinel<Iterator, LeftIterator> || sentinel<LeftIterator, Iterator>)
					return compare_iterator(left_iterator, other);
			}
			else
			{
				if constexpr (sentinel<Iterator, RightIterator> || sentinel<RightIterator, Iterator>)
					return compare_iterator(right_iterator, other);
			}
			return true;
		}
	};

	struct dont_check_heights_tag_t {};
	constexpr dont_check_heights_tag_t dont_check_heights_tag{};

	template <typename LeftMatrixView, typename RightMatrixView>
	class augmented_matrix_view
	{
		LeftMatrixView left;
		RightMatrixView right;

	public:
		template <matrix_view LeftMatrixViewAny, matrix_view RightMatrixViewAny>
		augmented_matrix_view(LeftMatrixViewAny&& left, RightMatrixViewAny&& right)
			: augmented_matrix_view(std::forward<LeftMatrixViewAny>(left), std::forward<RightMatrixViewAny>(right), dont_check_heights_tag)
		{
			if (height(left) != height(right))
				throw "incompatible heights";
		}
		template <matrix_view LeftMatrixViewAny, matrix_view RightMatrixViewAny>
		augmented_matrix_view(LeftMatrixViewAny&& left, RightMatrixViewAny&& right, dont_check_heights_tag_t)
			: left(simple_matrix_view(left))
			, right(simple_matrix_view(right))
		{}

		auto begin() const
		{
			return augmented_matrix_view_iterator(PPmatrix::begin(left), PPmatrix::width(left), PPmatrix::begin(right), PPmatrix::width(right));
		}
		auto end() const
		{
			return augmented_matrix_view_iterator(PPmatrix::end(left), PPmatrix::width(left), PPmatrix::end(right), PPmatrix::width(right));
		}
		auto width() const
		{
			return PPmatrix::width(left) + PPmatrix::width(right);
		}
		auto size() const
		{
			return PPmatrix::size(left) + PPmatrix::size(right);
		}
	};
	template <typename LeftMatrixViewAny, typename RightMatrixViewAny>
	augmented_matrix_view(LeftMatrixViewAny&& left, RightMatrixViewAny&& right)
		->augmented_matrix_view<
		decltype(simple_matrix_view(left)),
		decltype(simple_matrix_view(right))>;
}
