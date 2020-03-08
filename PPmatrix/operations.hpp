#pragma once
#include <array>
#include <algorithm>
#include <vector>
#include <numeric>
#include <type_traits>
#include "view.hpp"
#include "number_width.hpp"
#include "compressed_pair.hpp"
#include "skip_view.hpp"
#include "shift_view.hpp"
#include "fill.hpp"
#include "accumulate.hpp"
#include "find.hpp"
#include "rational.hpp"
#include "swap_ranges.hpp"
#include "max_element.hpp"
#include "transform_view.hpp"
#include "copy.hpp"
#include "functor.hpp"
#include "static_view.hpp"
#include "prev.hpp"
#include "augmented_matrix_view.hpp"
#include "matrix_view.hpp"

namespace PPmatrix
{
	namespace flag
	{
		using bitmask = std::size_t;

		constexpr bitmask		none = 0 << 0;
		constexpr bitmask      range = 1 << 0;
		constexpr bitmask     square = 1 << 1;
		constexpr bitmask       same = 1 << 2;
		constexpr bitmask		 one = 1 << 3;
		constexpr bitmask	    size = 1 << 4;
		constexpr bitmask triangular = 1 << 5;
		constexpr bitmask	  height = 1 << 6;
	}
	constexpr bool flag_set(flag::bitmask bitmask, std::size_t flag)
	{
		return (bitmask & flag) != 0;
	}

	template <matrix_view MatrixView>
	constexpr bool same_size(const MatrixView& A, const MatrixView& B)
	{
		return size(A) == size(B) && width(A) == width(B);
	}

	namespace detail
	{
		template <iterator Iterator>
		class row
		{
			std::size_t width_;
			Iterator begin_;
		public:
			template <matrix_view MatrixView>
			constexpr row(MatrixView&& matrix, std::size_t index)
				: width_(width(matrix))
				, begin_(PPmatrix::begin(matrix) + index * width_)
			{}
			constexpr iterator begin() const
			{
				return begin_;
			}
			constexpr iterator end() const
			{
				return begin_ + width_;
			}
			constexpr auto& operator*()
			{
				return *this;
			}
			constexpr auto& operator+=(std::size_t count)
			{
				begin_ += count * width_;
				return *this;
			}
			constexpr auto& operator-=(std::size_t count)
			{
				begin_ -= count * width_;
				return *this;
			}
			constexpr bool operator!=(Iterator other) const
			{
				return begin_ != other;
			}
			constexpr bool operator!=(row other) const
			{
				return begin_ != other.begin_;
			}
		};
		template <matrix_view MatrixView>
		row(MatrixView&&, std::size_t) -> row<begin_t<MatrixView>>;

		template <iterator Iterator>
		class column
		{
			std::size_t width_;
			Iterator begin_;
			Iterator end_;
		public:
			template <matrix_view MatrixView>
			column(MatrixView&& matrix, std::size_t index)
				: width_(width(matrix))
				, begin_(PPmatrix::begin(matrix) + index)
				, end_(PPmatrix::end(matrix) + index)
			{}
			constexpr iterator begin() const
			{
				return begin_ & skip(width_);
			}
			constexpr iterator end() const
			{
				return end_ & skip(width_);
			}
			constexpr auto& operator*()
			{
				return *this;
			}
			constexpr auto& operator+=(std::size_t count)
			{
				begin_ += count;
				end_ += count;
				return *this;
			}
			constexpr auto& operator-=(std::size_t count)
			{
				begin_ -= count;
				end_ -= count;
				return *this;
			}
			template <iterator OtherIterator>
			constexpr bool operator!=(OtherIterator other) const
			{
				return compare_iterator(begin_, other);
			}
		};
		template <matrix_view MatrixView>
		column(MatrixView&&, std::size_t) -> column<begin_t<MatrixView>>;

		template <iterator Iterator>
		class column_sentinel
		{
			skip_iterator<Iterator> begin_;
			Iterator end_;
		public:
			template <matrix_view MatrixView>
			column_sentinel(MatrixView&& matrix, std::size_t index)
				: begin_((PPmatrix::begin(matrix) + index) & skip(width(matrix)))
				, end_(PPmatrix::end(matrix) + index)
			{}
			constexpr iterator begin() const
			{
				return begin_;
			}
			constexpr iterator end() const
			{
				return end_;
			}
			constexpr auto& operator*()
			{
				return *this;
			}
			constexpr auto& operator+=(std::size_t count)
			{
				begin_.base() += count;
				end_ += count;
				return *this;
			}
			constexpr auto& operator-=(std::size_t count)
			{
				begin_.base() -= count;
				end_ -= count;
				return *this;
			}
			template <iterator OtherIterator>
			constexpr bool operator!=(OtherIterator other) const
			{
				return compare_iterator(begin_, other);
			}
		};
		template <matrix_view MatrixView>
		column_sentinel(MatrixView&&, std::size_t) -> column_sentinel<begin_t<MatrixView>>;
	}

	template <matrix_view MatrixView>
	constexpr auto& element(MatrixView&& matrix, std::size_t row, std::size_t column, std::size_t width)
	{
		return *(begin(matrix) + row * width + column);
	}
	template <matrix_view MatrixView>
	constexpr auto& element(MatrixView&& matrix, std::size_t row, std::size_t column)
	{
		return element(matrix, row, column, width(matrix));
	}

	template <matrix_view MatrixView>
	constexpr view row(MatrixView&& matrix, std::size_t index)
	{
		return detail::row(matrix, index);
	}
	template <matrix_view MatrixView>
	constexpr view row_sentinel(MatrixView&& matrix, std::size_t index)
	{
		return detail::row(matrix, index);
	}
	template <matrix_view MatrixView>
	constexpr view column(MatrixView&& matrix, std::size_t index)
	{
		return detail::column(matrix, index);
	}
	template <matrix_view MatrixView>
	constexpr view column_sentinel(MatrixView&& matrix, std::size_t index)
	{
		return detail::column_sentinel(matrix, index);
	}

	template <matrix_view MatrixView>
	constexpr iterator rows_begin(MatrixView&& matrix)
	{
		return row(matrix, 0);
	}
	template <matrix_view MatrixView>
	constexpr iterator rows_end_sentinel(MatrixView&& matrix)
	{
		return end(matrix);
	}
	template <matrix_view MatrixView>
	constexpr iterator rows_end(MatrixView&& matrix)
	{
		return row(matrix, height(matrix));
	}
	template <matrix_view MatrixView>
	constexpr view rows(MatrixView&& matrix)
	{
		return rows_begin(matrix) ^ rows_end(matrix);
	}
	template <matrix_view MatrixView>
	constexpr view rows_sentinel(MatrixView&& matrix)
	{
		return rows_begin(matrix) ^ rows_end_sentinel(matrix);
	}

	template <matrix_view MatrixView>
	constexpr iterator columns_begin(MatrixView&& matrix)
	{
		return column(matrix, 0);
	}
	template <matrix_view MatrixView>
	constexpr iterator columns_end_sentinel(MatrixView&& matrix)
	{
		return begin(matrix) + width(matrix);
	}
	template <matrix_view MatrixView>
	constexpr iterator columns_end(MatrixView&& matrix)
	{
		return column(matrix, width(matrix));
	}
	template <matrix_view MatrixView>
	constexpr view columns(MatrixView&& matrix)
	{
		return columns_begin(matrix) ^ columns_end(matrix);
	}
	template <matrix_view MatrixView>
	constexpr view columns_sentinel(MatrixView&& matrix)
	{
		return columns_begin(matrix) ^ columns_end_sentinel(matrix);
	}

	template <matrix_view MatrixView>
	constexpr view main_diagonal_sentinel(MatrixView&& matrix)
	{
		return (begin(matrix) & skip(width(matrix) + 1))
			^ (begin(matrix) + std::min(width(matrix), height(matrix)) * (width(matrix) + 1));
	}

	template <matrix_view MatrixView>
	constexpr void identity_matrix(MatrixView& matrix)
	{
		fill(matrix, 0);
		for (auto&& d : main_diagonal_sentinel(matrix))
			d = 1;
	}

	template <matrix_view MatrixView>
	constexpr bool is_square(const MatrixView& matrix)
	{
		return height(matrix) == width(matrix);
	}
	template <matrix_view MatrixView>
	constexpr bool is_upper_triangular(const MatrixView& matrix)
	{
		if (!is_square(matrix))
			return false;

		for (std::size_t i = 1; i != width(matrix); ++i)
			for (std::size_t j = 0; j != i; ++j)
				if (element(matrix, i, j) != 0)
					return false;

		return true;
	}
	template <matrix_view MatrixView>
	constexpr bool is_lower_triangular(const MatrixView& matrix)
	{
		if (!is_square(matrix))
			return false;

		for (std::size_t i = 0; i != width(matrix) - 1; ++i)
			for (std::size_t j = i + 1; j != width(matrix); ++j)
				if (element(matrix, i, j) != 0)
					return false;

		return true;
	}
	template <matrix_view MatrixView>
	constexpr bool is_triangular(const MatrixView& matrix)
	{
		return is_upper_triangular(matrix) || is_lower_triangular(matrix);
	}
	template <matrix_view MatrixView>
	constexpr bool is_diagonal(const MatrixView& matrix)
	{
		return is_upper_triangular(matrix) && is_lower_triangular(matrix);
	}

	template <flag::bitmask flags = flag::none, matrix_view MatrixViewA, matrix_view MatrixViewB, matrix_view MatrixViewResult>
	constexpr void multiply(const MatrixViewA& A, const MatrixViewB& B, MatrixViewResult& result)
	{
		if constexpr (flag_set(flags, flag::size))
			if (width(A) != height(B)
				|| height(A) != height(result)
				|| width(B) != width(result))
				throw "incompatible sizes";

		for (std::size_t i = 0; i != height(result); ++i)
			for (std::size_t j = 0; j != width(result); ++j)
				element(result, i, j) = inner_product(row_sentinel(A, i), column_sentinel(B, j));
	}
	template <matrix_view MatrixView, typename T>
	constexpr void multiply(MatrixView& matrix, const T& scalar)
	{
		for (auto&& element : matrix)
			element *= scalar;
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixViewA, matrix_view MatrixViewB>
	constexpr MatrixViewA& add(MatrixViewA& A, const MatrixViewB& B)
	{
		if constexpr (flag_set(flags, flag::size))
			if (!same_size(A, B))
				throw "incompatible sizes";

		zip(A, B, functor::add);

		return A;
	}

	template <bool const_square = false, matrix_view MatrixView>
	constexpr void transpose(MatrixView& matrix, bool square)
	{
		if constexpr (const_square)
		{
			for (std::size_t i = 0; i != width(matrix); ++i)
				for (std::size_t j = i + 1; j != width(matrix); ++j)
					std::swap(element(matrix, i, j), element(matrix, j, i));
		}
		else
		{
			if (square)
				transpose<true>(matrix, true);
			else
			{
				// https://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose/

				view_base_t<MatrixView> t;
				std::size_t next;
				std::size_t cycleBegin;
				std::size_t i;
				std::size_t s = size(matrix) - 1;
				std::vector<bool> b(size(matrix));
				fill(b, false);
				b[0] = b[s] = true;
				i = 1;
				while (i != s)
				{
					cycleBegin = i;
					t = begin(matrix)[i];
					do
					{
						next = (i * height(matrix)) % s;
						std::swap(begin(matrix)[next], t);
						b[i] = true;
						i = next;
					} while (i != cycleBegin);

					for (i = 1; i != s && b[i]; ++i);
				}
			}
		}
	}
	template <bool const_square = false, matrix_view MatrixView>
	constexpr void transpose(MatrixView& matrix)
	{
		if constexpr (const_square)
			transpose<true, MatrixView>(matrix, true);
		else
			return transpose<false, MatrixView>(matrix, is_square(matrix));
	}

	namespace detail
	{
		template <bool do_check = true, matrix_view MatrixView, typename... T>
		constexpr void check_row_indices([[maybe_unused]] MatrixView& matrix, [[maybe_unused]] T... indices)
		{
			if constexpr (do_check)
				if (((indices >= height(matrix)) || ...))
					throw "invalid index";
		}
	}

	template <flag::bitmask flags = flag::none, matrix_view MatrixView>
	constexpr void swap_rows(MatrixView& matrix, std::size_t i1, std::size_t i2)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

		if constexpr (flag_set(flags, flag::same))
			if (i1 == i2)
				return;

		swap_ranges(row_sentinel(matrix, i1), row_sentinel(matrix, i2));
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView, typename T>
	constexpr void multiply_row(MatrixView& matrix, std::size_t index, const T& scalar)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		if constexpr (flag_set(flags, flag::one))
			if (scalar == 1)
				return;

		for (auto& element : row_sentinel(matrix, index))
			element *= scalar;
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView, typename T>
	constexpr void divide_row(MatrixView& matrix, std::size_t index, const T& scalar)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		if constexpr (flag_set(flags, flag::one))
			if (scalar == 1)
				return;

		for (auto& element : row_sentinel(matrix, index))
			element /= scalar;
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView>
	constexpr view_base_t<MatrixView> simplify_row(MatrixView& matrix, std::size_t index)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		auto R = row_sentinel(matrix, index);
		auto i = find(R, [](auto&& x) { return x != 0; });
		view_base_t<MatrixView> gcd = 0;
		if (i != end(R))
		{
			gcd = accumulate(R,
				[](auto&& x, auto&& y)
				{
					if (y == 0)
						return x;
					return std::gcd(x, y);
				}, *i);

			for (auto&& element : R)
				element /= gcd;
		}
		return gcd;
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView, typename T>
	constexpr void add_rows(MatrixView& matrix, std::size_t i1, const T& scalar, std::size_t i2)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

		zip(row_sentinel(matrix, i2), row_sentinel(matrix, i1),
			[&scalar](auto&& x, auto&& y) { x += scalar * y; });
	}

	template <bool reduced = false, bool calculate_determinant = false, bool calculate_rank = false, matrix_view MatrixView>
	constexpr auto REF(MatrixView& matrix)
	{
		using T = view_base_t<MatrixView>;
		// GCC complains about static in a constexpr function
		/*static*/ constexpr bool integral = std::is_integral_v<T>;

		// relying on compiler to optimize out unused local variables
		// otherwise causes undeclared variable error
		[[maybe_unused]]
		std::conditional_t<calculate_determinant,
			std::conditional_t<integral,
			rational<T>,
			T>,
			empty> determinant;

		if constexpr (calculate_determinant)
			determinant = 1;

		std::size_t r = 0;
		std::size_t c = 0;

		while (r != height(matrix) && c != width(matrix))
		{
			std::size_t k = r;
			for (; k != height(matrix) && element(matrix, k, c) == 0; ++k);

			if (k != height(matrix))
			{
				if (k != r)
				{
					swap_rows(matrix, k, r);
					if constexpr (calculate_determinant)
						determinant = -determinant;
				}

				if constexpr (integral)
				{
					if constexpr (calculate_determinant)
						determinant *= simplify_row(matrix, r);
					else
						simplify_row(matrix, r);
				}

				auto i_element = element(matrix, r, c);

				if constexpr ((!reduced || !integral) && calculate_determinant)
					determinant *= i_element;

				std::size_t j;
				if constexpr (reduced)
					j = 0;
				else
					j = r + 1;

				for (; j != height(matrix); ++j)
				{
					if constexpr (reduced)
						if (j == r)
							continue;

					auto j_element = element(matrix, j, c);
					if (j_element != 0)
					{
						T y;

						if constexpr (integral)
						{
							auto lcm = std::lcm(i_element, j_element);
							auto x = lcm / j_element;
							multiply_row(matrix, j, x);
							if constexpr (calculate_determinant)
								determinant /= x;

							y = lcm / i_element;
						}
						else
							y = j_element / i_element;

						add_rows(matrix, r, -y, j);
					}
				}

				if constexpr (reduced && integral)
				{
					for (std::size_t i = 0; i != height(matrix); ++i)
					{
						if constexpr (calculate_determinant)
							determinant *= simplify_row(matrix, i);
						else
							simplify_row(matrix, i);
					}
				}

				++r;
			}
			else
			{
				if constexpr (calculate_determinant)
					determinant = 0;
			}

			++c;
		}

		if constexpr (calculate_rank && calculate_determinant)
			return std::make_pair(determinant, r);
		else if constexpr (calculate_determinant)
			return static_cast<T>(determinant);
		else if constexpr (calculate_rank)
			return r;
		else
			return;
	}
	template <matrix_view MatrixView>
	constexpr auto REF_det(MatrixView& matrix)
	{
		return REF<false, true, false>(matrix);
	}
	template <matrix_view MatrixView>
	constexpr std::size_t rank(MatrixView& matrix)
	{
		return REF<false, false, true>(matrix);
	}
	template <typename MatrixView>
	constexpr std::size_t REF_reduced_rank(MatrixView& matrix)
	{
		return REF<true, false, true>(matrix);
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView>
	constexpr auto determinant(MatrixView& matrix, [[maybe_unused]] bool triangular)
	{
		if constexpr (flag_set(flags, flag::square))
			if (!is_square(matrix))
				throw "determinant is defined only for square matrices";

		if constexpr (flag_set(flags, flag::triangular))
		{
			view_base_t<MatrixView> determinant = 1;
			for (auto&& d : main_diagonal_sentinel(matrix))
			{
				determinant *= d;
				if (determinant == 0)
					break;
			}
			return determinant;
		}
		else
		{
			if (!triangular)
				return REF_det(matrix);
			else
				return determinant<flags | flag::triangular>(matrix, true);
		}
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView>
	constexpr auto determinant(MatrixView& matrix)
	{
		if constexpr (flag_set(flags, flag::triangular))
			return determinant<flags>(matrix, true);
		else
			return determinant<flags>(matrix, is_triangular(matrix));
	}
	template <flag::bitmask flags = flag::none, matrix_view MatrixView, matrix_view MatrixViewVector>
	constexpr auto solve_linear_equations(MatrixView& M, MatrixViewVector& v)
	{
		using return_t = std::make_signed_t<std::size_t>;

		if constexpr (flag_set(flags, flag::height))
			if (height(v) != height(M) || width(v) != 1)
				throw "invalid size of vector";

		auto w = width(M);
		augmented_matrix_view matrix(M, v);

		auto r = REF_reduced_rank(matrix);

		std::size_t sub_rank = 0;
		for (std::size_t j = 0; j != w && sub_rank != height(matrix); ++j)
			if (element(matrix, sub_rank, j) != 0)
				++sub_rank;

		if (sub_rank < r)
			return return_t(-1);
		else
		{
			if (r == w)
			{
				auto k = begin(v);
				for (std::size_t i = 0; k != end(v); ++i, ++k)
					*k = element(matrix, i, w) / element(matrix, i, i);
			}
			return return_t(w - r);
		}
	}

	namespace detail
	{
		template <matrix_view MatrixView, typename Printer>
		std::ostream& print_helper(std::ostream& out, const MatrixView& matrix, Printer printer)
		{
			for (auto&& row : rows_sentinel(matrix))
			{
				out << '|';
				auto i = begin(row);
				for (; i != PPmatrix::prev(end(row)); ++i)
				{
					printer(*i);
					out << ' ';
				}
				printer(*i);
				out << "|\n";
			}

			return out;
		}
	}

	template <matrix_view MatrixView>
	std::ostream& pretty_print(std::ostream& out, const MatrixView& matrix)
	{
		std::size_t max_width = *max_element(matrix | transform(functor::number_width));

		return detail::print_helper(out, matrix,
			[&out, max_width](const auto& element)
			{
				for (std::size_t i = 0; i != max_width - functor::number_width(element); ++i)
					out << ' ';
				out << element;
			});
	}

	template <matrix_view MatrixView>
	std::ostream& basic_print(std::ostream& out, const MatrixView& matrix)
	{
		return detail::print_helper(out, matrix,
			[&out](const auto& element)
			{
				out << element;
			});
	}
}
