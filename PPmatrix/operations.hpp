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
#include "default_element.hpp"

namespace PPmatrix
{
	namespace flag
	{
		using bitmask = size_t;

		constexpr bitmask		none = 0 << 0;
		constexpr bitmask      range = 1 << 0;
		constexpr bitmask     square = 1 << 1;
		constexpr bitmask       same = 1 << 2;
		constexpr bitmask		 one = 1 << 3;
		constexpr bitmask	    size = 1 << 4;
		constexpr bitmask triangular = 1 << 5;
		constexpr bitmask	  height = 1 << 6;
	}
	constexpr bool flag_set(flag::bitmask bitmask, size_t flag)
	{
		return (bitmask & flag) != 0;
	}

	constexpr bool same_size(matrix_view auto&& A, matrix_view auto&& B)
	{
		return size(A) == size(B) && width(A) == width(B);
	}

	// behaves as a view as well as an iterator
	template <iterator Iterator>
	class row_t
	{
		size_t width_;
		Iterator begin_;
	public:
		constexpr row_t(matrix_view auto&& matrix, size_t index)
			: width_(width(matrix))
			, begin_(PPmatrix::begin(matrix) + index * width_)
		{}
		constexpr iterator auto begin() const
		{
			return begin_;
		}
		constexpr iterator auto end() const
		{
			return begin_ + width_;
		}
		constexpr auto& operator*()
		{
			return *this;
		}
		constexpr auto& operator+=(size_t count)
		{
			begin_ += count * width_;
			return *this;
		}
		constexpr auto& operator-=(size_t count)
		{
			begin_ -= count * width_;
			return *this;
		}
		constexpr bool operator==(iterator auto other) const
		{
			return begin_ == other;
		}
	};
	template <matrix_view MatrixView>
	row_t(MatrixView&&, size_t) -> row_t<begin_t<MatrixView>>;

	// behaves as a view as well as an iterator
	template <iterator Iterator>
	class column_t
	{
		size_t width_;
		Iterator begin_;
		Iterator end_;
	public:
		column_t(matrix_view auto&& matrix, size_t index)
			: width_(width(matrix))
			, begin_(PPmatrix::begin(matrix) + index)
			, end_(PPmatrix::end(matrix) + index)
		{}
		constexpr iterator auto begin() const
		{
			return begin_ & skip(width_);
		}
		constexpr iterator auto end() const
		{
			return end_ & skip(width_);
		}
		constexpr auto& operator*()
		{
			return *this;
		}
		constexpr auto& operator+=(size_t count)
		{
			begin_ += count;
			end_ += count;
			return *this;
		}
		constexpr auto& operator-=(size_t count)
		{
			begin_ -= count;
			end_ -= count;
			return *this;
		}
		constexpr bool operator==(iterator auto other) const
		{
			return begin_ == other;
		}
	};
	template <matrix_view MatrixView>
	column_t(MatrixView&&, size_t) -> column_t<begin_t<MatrixView>>;

	// behaves as a view as well as an iterator
	template <iterator Iterator>
	class column_sentinel_t
	{
		skip_iterator<Iterator> begin_;
		Iterator end_;
	public:
		column_sentinel_t(matrix_view auto&& matrix, size_t index)
			: begin_((PPmatrix::begin(matrix) + index) & skip(width(matrix)))
			, end_(PPmatrix::end(matrix) + index)
		{}
		constexpr iterator auto begin() const
		{
			return begin_;
		}
		constexpr auto end() const
		{
			return end_;
		}
		constexpr auto& operator*()
		{
			return *this;
		}
		constexpr auto& operator+=(size_t count)
		{
			begin_.base() += count;
			end_ += count;
			return *this;
		}
		constexpr auto& operator-=(size_t count)
		{
			begin_.base() -= count;
			end_ -= count;
			return *this;
		}
		constexpr bool operator==(iterator auto other) const
		{
			return begin_ == other;
		}
	};
	template <matrix_view MatrixView>
	column_sentinel_t(MatrixView&&, size_t) -> column_sentinel_t<begin_t<MatrixView>>;

	constexpr auto& element(matrix_view auto&& matrix, size_t row, size_t column, size_t width)
	{
		return *(begin(matrix) + row * width + column);
	}
	constexpr auto& element(matrix_view auto&& matrix, size_t row, size_t column)
	{
		return element(matrix, row, column, width(matrix));
	}

	constexpr view auto row(matrix_view auto&& matrix, size_t index)
	{
		return row_t(matrix, index);
	}
	constexpr view auto row_sentinel(matrix_view auto&& matrix, size_t index)
	{
		return row_t(matrix, index);
	}
	constexpr view auto column(matrix_view auto&& matrix, size_t index)
	{
		return column_t(matrix, index);
	}
	constexpr view auto column_sentinel(matrix_view auto&& matrix, size_t index)
	{
		return column_sentinel_t(matrix, index);
	}

	constexpr iterator auto rows_begin(matrix_view auto&& matrix)
	{
		return row(matrix, 0);
	}
	constexpr auto rows_end_sentinel(matrix_view auto&& matrix)
	{
		return end(matrix);
	}
	constexpr iterator auto rows_end(matrix_view auto&& matrix)
	{
		return row(matrix, height(matrix));
	}
	constexpr view auto rows(matrix_view auto&& matrix)
	{
		return rows_begin(matrix) ^ rows_end(matrix);
	}
	constexpr view auto rows_sentinel(matrix_view auto&& matrix)
	{
		return rows_begin(matrix) ^ rows_end_sentinel(matrix);
	}

	constexpr iterator auto columns_begin(matrix_view auto&& matrix)
	{
		return column(matrix, 0);
	}
	constexpr auto columns_end_sentinel(matrix_view auto&& matrix)
	{
		return begin(matrix) + width(matrix);
	}
	constexpr iterator auto columns_end(matrix_view auto&& matrix)
	{
		return column(matrix, width(matrix));
	}
	constexpr view auto columns(matrix_view auto&& matrix)
	{
		return columns_begin(matrix) ^ columns_end(matrix);
	}
	constexpr view auto columns_sentinel(matrix_view auto&& matrix)
	{
		return columns_begin(matrix) ^ columns_end_sentinel(matrix);
	}

	constexpr view auto main_diagonal_sentinel(matrix_view auto&& matrix)
	{
		return (begin(matrix) & skip(width(matrix) + 1))
			^ (begin(matrix) + std::min(width(matrix), height(matrix)) * (width(matrix) + 1));
	}

	constexpr void make_identity_matrix(matrix_view auto&& matrix)
	{
		fill(matrix, 0);
		for (auto&& d : main_diagonal_sentinel(matrix))
			d = 1;
	}

	constexpr bool is_square(matrix_view auto&& matrix)
	{
		return height(matrix) == width(matrix);
	}
	constexpr bool is_upper_triangular(matrix_view auto&& matrix)
	{
		if (!is_square(matrix))
			return false;

		for (size_t i = 1; i != width(matrix); ++i)
			for (size_t j = 0; j != i; ++j)
				if (element(matrix, i, j) != 0)
					return false;

		return true;
	}
	constexpr bool is_lower_triangular(matrix_view auto&& matrix)
	{
		if (!is_square(matrix))
			return false;

		for (size_t i = 0; i != width(matrix) - 1; ++i)
			for (size_t j = i + 1; j != width(matrix); ++j)
				if (element(matrix, i, j) != 0)
					return false;

		return true;
	}
	constexpr bool is_triangular(matrix_view auto&& matrix)
	{
		return is_upper_triangular(matrix) || is_lower_triangular(matrix);
	}
	constexpr bool is_diagonal(matrix_view auto&& matrix)
	{
		return is_upper_triangular(matrix) && is_lower_triangular(matrix);
	}

	template <flag::bitmask flags = flag::none>
	constexpr void multiply(matrix_view auto&& A, matrix_view auto&& B, matrix_view auto&& result)
	{
		if constexpr (flag_set(flags, flag::size))
			if (width(A) != height(B)
				|| height(A) != height(result)
				|| width(B) != width(result))
				throw "incompatible sizes";

		for (size_t i = 0; i != height(result); ++i)
			for (size_t j = 0; j != width(result); ++j)
				element(result, i, j) = inner_product(row_sentinel(A, i), column_sentinel(B, j));
	}
	constexpr void multiply(matrix_view auto&& matrix, const auto& scalar)
	{
		for (auto&& element : matrix)
			element *= scalar;
	}
	template <flag::bitmask flags = flag::none>
	constexpr void add(matrix_view auto&& A, matrix_view auto&& B)
	{
		if constexpr (flag_set(flags, flag::size))
			if (!same_size(A, B))
				throw "incompatible sizes";

		zip(A, B, functor::add);
	}

	template <bool const_square = false>
	constexpr void transpose(matrix_view auto&& matrix, bool square)
	{
		if constexpr (const_square)
		{
			for (size_t i = 0; i != width(matrix); ++i)
				for (size_t j = i + 1; j != width(matrix); ++j)
					std::swap(element(matrix, i, j), element(matrix, j, i));
		}
		else
		{
			if (square)
				transpose<true>(matrix, true);
			else
			{
				// https://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose/

				auto t = default_element(matrix);
				size_t next;
				size_t cycleBegin;
				size_t i;
				size_t s = size(matrix) - 1;
				std::vector<bool> b(size(matrix));
				b.assign(b.size(), false);
				b[0] = true;
				b[s] = true;
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
	template <bool const_square = false>
	constexpr void transpose(matrix_view auto&& matrix)
	{
		if constexpr (const_square)
			transpose<true>(matrix, true);
		else
			return transpose<false>(matrix, is_square(matrix));
	}

	namespace detail
	{
		template <bool do_check = true>
		constexpr void check_row_indices([[maybe_unused]] matrix_view auto&& matrix, [[maybe_unused]] auto... indices)
		{
			if constexpr (do_check)
				if (((indices >= height(matrix)) || ...))
					throw "invalid index";
		}
	}

	template <flag::bitmask flags = flag::none>
	constexpr void swap_rows(matrix_view auto&& matrix, size_t i1, size_t i2)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

		if constexpr (flag_set(flags, flag::same))
			if (i1 == i2)
				return;

		swap_ranges(row_sentinel(matrix, i1), row_sentinel(matrix, i2));
	}
	template <flag::bitmask flags = flag::none>
	constexpr void multiply_row(matrix_view auto&& matrix, size_t index, const auto& scalar)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		if constexpr (flag_set(flags, flag::one))
			if (scalar == 1)
				return;

		for (auto&& element : row_sentinel(matrix, index))
			element *= scalar;
	}
	template <flag::bitmask flags = flag::none>
	constexpr void divide_row(matrix_view auto&& matrix, size_t index, const auto& scalar)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		if constexpr (flag_set(flags, flag::one))
			if (scalar == 1)
				return;

		for (auto& element : row_sentinel(matrix, index))
			element /= scalar;
	}
	template <flag::bitmask flags = flag::none>
	constexpr auto simplify_row(matrix_view auto&& matrix, size_t index)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

		auto R = row_sentinel(matrix, index);
		auto i = find(R, [](auto&& x) { return x != 0; });
		auto gcd = default_element(matrix);
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
	template <flag::bitmask flags = flag::none>
	constexpr void add_rows(matrix_view auto&& matrix, size_t i1, const auto& scalar, size_t i2)
	{
		detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

		zip(row_sentinel(matrix, i2), row_sentinel(matrix, i1),
			[&scalar](auto&& x, auto&& y) { x += scalar * y; });
	}

	namespace detail
	{
		// workaround for rational<T> requires std::integral<T>
		template <std::integral T>
		consteval auto determinant_t_helper() -> std::type_identity<rational<T>>;
		template <typename T>
		consteval auto determinant_t_helper() -> std::type_identity<T>;
		template <typename T>
		using determinant_t = decltype(determinant_t_helper<T>())::type;
	}

	template <bool reduced = false, bool calculate_determinant = false, bool calculate_rank = false>
	constexpr auto REF(matrix_view auto&& matrix)
	{
		using T = view_base_t<decltype(matrix)>;
		// GCC complains about static in a constexpr function
		/*static*/ constexpr bool integral = std::integral<T>;

		// relying on compiler to optimize out unused local variables
		// otherwise causes undeclared variable error
		[[maybe_unused]]
		std::conditional_t<calculate_determinant,
			detail::determinant_t<T>,
			empty> determinant;

		if constexpr (calculate_determinant)
			determinant = 1;

		size_t r = 0;
		size_t c = 0;

		while (r != height(matrix) && c != width(matrix))
		{
			size_t k = r;
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

				size_t j;
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
					for (size_t i = 0; i != height(matrix); ++i)
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
	constexpr auto REF_det(matrix_view auto&& matrix)
	{
		return REF<false, true, false>(matrix);
	}
	constexpr size_t rank(matrix_view auto&& matrix)
	{
		return REF<false, false, true>(matrix);
	}
	constexpr size_t REF_reduced_rank(matrix_view auto&& matrix)
	{
		return REF<true, false, true>(matrix);
	}
	template <flag::bitmask flags = flag::none>
	constexpr auto determinant(matrix_view auto&& matrix, [[maybe_unused]] bool triangular)
	{
		if constexpr (flag_set(flags, flag::square))
			if (!is_square(matrix))
				throw "determinant is defined only for square matrices";

		if constexpr (flag_set(flags, flag::triangular))
		{
			auto determinant = default_element(matrix);
			determinant = 1;
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
	template <flag::bitmask flags = flag::none>
	constexpr auto determinant(matrix_view auto&& matrix)
	{
		if constexpr (flag_set(flags, flag::triangular))
			return determinant<flags>(matrix, true);
		else
			return determinant<flags>(matrix, is_triangular(matrix));
	}
	template <flag::bitmask flags = flag::none>
	constexpr auto solve_linear_equations(matrix_view auto&& M, matrix_view auto&& v)
	{
		using return_t = std::make_signed_t<size_t>;

		if constexpr (flag_set(flags, flag::height))
			if (height(v) != height(M) || width(v) != 1)
				throw "invalid size of vector";

		auto w = width(M);
		augmented_matrix_view matrix(M, v);

		auto r = REF_reduced_rank(matrix);

		size_t sub_rank = 0;
		for (size_t j = 0; j != w && sub_rank != height(matrix); ++j)
			if (element(matrix, sub_rank, j) != 0)
				++sub_rank;

		if (sub_rank < r)
			return return_t(-1);
		else
		{
			if (r == w)
			{
				auto k = begin(v);
				for (size_t i = 0; k != end(v); ++i, ++k)
					*k = element(matrix, i, w) / element(matrix, i, i);
			}
			return return_t(w - r);
		}
	}

	namespace detail
	{
		std::ostream& print_helper(std::ostream& out, matrix_view auto&& matrix, auto printer)
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

	std::ostream& pretty_print(std::ostream& out, matrix_view auto&& matrix)
	{
		size_t max_width = *max_element(matrix | transform(functor::number_width));

		return detail::print_helper(out, matrix,
			[&out, max_width](const auto& element)
			{
				for (size_t i = 0; i != max_width - functor::number_width(element); ++i)
					out << ' ';
				out << element;
			});
	}

	std::ostream& basic_print(std::ostream& out, matrix_view auto&& matrix)
	{
		return detail::print_helper(out, matrix,
			[&out](const auto& element)
			{
				out << element;
			});
	}
}
