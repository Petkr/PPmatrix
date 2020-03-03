#pragma once
#include <cstddef>
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
#include "execute.hpp"
#include "prev.hpp"
#include "augmented_matrix_view.hpp"

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

template <typename Iterator, typename Sentinel>
class simple_matrix_view
{
	std::size_t width_;
	compressed_pair<Iterator, Sentinel> pair;

public:
	template <typename View>
	constexpr simple_matrix_view(View&& view, std::size_t width)
		: width_(width)
		, pair{ ::begin(view), ::end(view) }
	{}
	template <typename MatrixView>
	constexpr simple_matrix_view(MatrixView&& view)
		: simple_matrix_view(view, ::width(view))
	{}
	constexpr simple_matrix_view(Iterator begin, Sentinel end, std::size_t width)
		: width_(width)
		, pair{ begin, end }
	{}
	constexpr auto begin() const
	{
		return pair.first;
	}
	constexpr auto end() const
	{
		return pair.second;
	}
	constexpr auto width() const
	{
		return width_;
	}

	constexpr auto set_width(std::size_t width)
	{
		width_ = width;
	}
};
template <typename MatrixView>
simple_matrix_view(MatrixView&&)->simple_matrix_view<begin_t<MatrixView>, end_t<MatrixView>>;
template <typename View>
simple_matrix_view(View&&, std::size_t) -> simple_matrix_view<begin_t<View>, end_t<View>>;

template <typename T>
concept has_width = requires (const T t)
{
	{ t.width() } -> std::size_t;
};
template <typename MatrixView>
constexpr std::size_t width(const MatrixView& matrix)
{
	if constexpr (has_width<MatrixView>)
		return matrix.width();
	else
		return 1;
}
template <typename MatrixView>
constexpr std::size_t height(const MatrixView& matrix)
{
	return ::size(matrix) / width(matrix);
}

template <typename MatrixView>
constexpr auto same_size(const MatrixView& A, const MatrixView& B)
{
	return size(A) == size(B) && width(A) == width(B);
}

namespace detail
{
	template <typename Iterator>
	class row
	{
		std::size_t width_;
		Iterator begin_;
	public:
		template <typename MatrixView>
		constexpr row(MatrixView&& matrix, std::size_t index)
			: width_(::width(matrix))
			, begin_(::begin(matrix) + index * width_)
		{}
		constexpr auto begin() const
		{
			return begin_;
		}
		constexpr auto end() const
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
		constexpr auto operator!=(Iterator other) const
		{
			return begin_ != other;
		}
		constexpr auto operator!=(row other) const
		{
			return begin_ != other.begin_;
		}
	};
	template <typename MatrixView>
	row(MatrixView&&, std::size_t) -> row<begin_t<MatrixView>>;

	template <typename Iterator>
	class column
	{
		std::size_t width_;
		Iterator begin_;
		Iterator end_;
	public:
		template <typename MatrixView>
		column(MatrixView&& matrix, std::size_t index)
			: width_(width(matrix))
			, begin_(::begin(matrix) + index)
			, end_(::end(matrix) + index)
		{}
		constexpr auto begin() const
		{
			return begin_ & skip(width_);
		}
		constexpr auto end() const
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
		constexpr auto operator!=(Iterator other) const
		{
			return begin_ != other;
		}
		constexpr auto operator!=(column other) const
		{
			return begin_ != other.begin_;
		}
	};
	template <typename MatrixView>
	column(MatrixView&&, std::size_t) -> column<begin_t<MatrixView>>;

	template <typename Iterator>
	class column_sentinel
	{
		skip_iterator<Iterator> begin_;
		Iterator end_;
	public:
		template <typename MatrixView>
		column_sentinel(MatrixView&& matrix, std::size_t index)
			: begin_((::begin(matrix) + index)& skip(width(matrix)))
			, end_(::end(matrix) + index)
		{}
		constexpr auto begin() const
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
		constexpr auto operator!=(Iterator other) const
		{
			return begin_ != other;
		}
		constexpr auto operator!=(column_sentinel other) const
		{
			return begin_ != other.begin_;
		}
	};
	template <typename MatrixView>
	column_sentinel(MatrixView&&, std::size_t) -> column_sentinel<begin_t<MatrixView>>;
}

template <typename MatrixView>
constexpr auto& element(MatrixView&& matrix, std::size_t row, std::size_t column, std::size_t width)
{
	return *(begin(matrix) + row * width + column);
}
template <typename MatrixView>
constexpr auto& element(MatrixView&& matrix, std::size_t row, std::size_t column)
{
	return element(matrix, row, column, width(matrix));
}

template <typename MatrixView>
constexpr auto row(MatrixView&& matrix, std::size_t index)
{
	return detail::row(matrix, index);
}
template <typename MatrixView>
constexpr auto row_sentinel(MatrixView&& matrix, std::size_t index)
{
	return detail::row(matrix, index);
}
template <typename MatrixView>
constexpr auto column(MatrixView&& matrix, std::size_t index)
{
	return detail::column(matrix, index);
}
template <typename MatrixView>
constexpr auto column_sentinel(MatrixView&& matrix, std::size_t index)
{
	return detail::column_sentinel(matrix, index);
}

template <typename MatrixView>
constexpr auto rows_begin(MatrixView&& matrix)
{
	return row(matrix, 0);
}
template <typename MatrixView>
constexpr auto rows_end_sentinel(MatrixView&& matrix)
{
	return end(matrix);
}
template <typename MatrixView>
constexpr auto rows_end(MatrixView&& matrix)
{
	return row(matrix, height(matrix));
}
template <typename MatrixView>
constexpr auto rows(MatrixView&& matrix)
{
	return rows_begin(matrix) ^ rows_end(matrix);
}
template <typename MatrixView>
constexpr auto rows_sentinel(MatrixView&& matrix)
{
	return rows_begin(matrix) ^ rows_end_sentinel(matrix);
}

template <typename MatrixView>
constexpr auto columns_begin(MatrixView&& matrix)
{
	return column(matrix, 0);
}
template <typename MatrixView>
constexpr auto columns_end_sentinel(MatrixView&& matrix)
{
	return begin(matrix) + width(matrix);
}
template <typename MatrixView>
constexpr auto columns_end(MatrixView&& matrix)
{
	return column(matrix, width(matrix));
}
template <typename MatrixView>
constexpr auto columns(MatrixView&& matrix)
{
	return columns_begin(matrix) ^ columns_end(matrix);
}
template <typename MatrixView>
constexpr auto columns_sentinel(MatrixView&& matrix)
{
	return columns_begin(matrix) ^ columns_end_sentinel(matrix);
}

template <typename MatrixView>
constexpr auto main_diagonal_sentinel(MatrixView&& matrix)
{
	return (begin(matrix) & skip(width(matrix) + 1))
		 ^ (begin(matrix) + std::min(width(matrix), height(matrix)) * (width(matrix) + 1));
}

template <typename MatrixView>
constexpr void identity_matrix(MatrixView& matrix)
{
	fill(matrix, 0);
	for (auto&& d : main_diagonal_sentinel(matrix))
		d = 1;
}

template <typename MatrixView>
constexpr auto is_square(const MatrixView& matrix)
{
	return height(matrix) == width(matrix);
}
template <typename MatrixView>
constexpr auto is_upper_triangular(const MatrixView& matrix)
{
	if (!is_square(matrix))
		return false;

	for (std::size_t i = 1; i != width(matrix); ++i)
		for (std::size_t j = 0; j != i; ++j)
			if (element(matrix, i, j) != 0)
				return false;

	return true;
}
template <typename MatrixView>
constexpr auto is_lower_triangular(const MatrixView& matrix)
{
	if (!is_square(matrix))
		return false;

	for (std::size_t i = 0; i != width(matrix) - 1; ++i)
		for (std::size_t j = i + 1; j != width(matrix); ++j)
			if (element(matrix, i, j) != 0)
				return false;

	return true;
}
template <typename MatrixView>
constexpr auto is_triangular(const MatrixView& matrix)
{
	return is_upper_triangular(matrix) || is_lower_triangular(matrix);
}
template <typename MatrixView>
constexpr auto is_diagonal(const MatrixView& matrix)
{
	return is_upper_triangular(matrix) && is_lower_triangular(matrix);
}

template <flag::bitmask flags = flag::none, typename MatrixViewA, typename MatrixViewB, typename MatrixViewResult>
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
template <typename MatrixView, typename T>
constexpr void multiply(MatrixView& matrix, const T& scalar)
{
	for (auto&& element : matrix)
		element *= scalar;
}
template <flag::bitmask flags = flag::none, typename MatrixViewA, typename MatrixViewB>
constexpr auto& add(MatrixViewA& A, const MatrixViewB& B)
{
	if constexpr (flag_set(flags, flag::size))
		if (!same_size(A, B))
			throw "incompatible sizes";

	zip(A, B, functor::add);

	return A;
}

template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix, bool square)
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

		return height(matrix);
	}
}
template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix)
{
	if constexpr (const_square)
		transpose<true, MatrixView>(matrix, true);
	else
		return transpose<false, MatrixView>(matrix, is_square(matrix));
}

namespace detail
{
	template <bool do_check = true, typename MatrixView, typename... T>
	constexpr void check_row_indices([[maybe_unused]] MatrixView& matrix, [[maybe_unused]] T... indices)
	{
		if constexpr (do_check)
			if (((indices >= height(matrix)) || ...))
				throw "invalid index";
	}
}

template <flag::bitmask flags = flag::none, typename MatrixView>
constexpr void swap_rows(MatrixView& matrix, std::size_t i1, std::size_t i2)
{
	detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

	if constexpr (flag_set(flags, flag::same))
		if (i1 == i2)
			return;

	swap_ranges(row_sentinel(matrix, i1), row_sentinel(matrix, i2));
}
template <flag::bitmask flags = flag::none, typename MatrixView, typename T>
constexpr void multiply_row(MatrixView& matrix, std::size_t index, const T& scalar)
{
	detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

	if constexpr (flag_set(flags, flag::one))
		if (scalar == 1)
			return;

	for (auto& element : row_sentinel(matrix, index))
		element *= scalar;
}
template <flag::bitmask flags = flag::none, typename MatrixView, typename T>
constexpr void divide_row(MatrixView& matrix, std::size_t index, const T& scalar)
{
	detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

	if constexpr (flag_set(flags, flag::one))
		if (scalar == 1)
			return;

	for (auto& element : row_sentinel(matrix, index))
		element /= scalar;
}
template <flag::bitmask flags = flag::none, typename MatrixView>
constexpr auto simplify_row(MatrixView& matrix, std::size_t index)
{
	detail::check_row_indices<flag_set(flags, flag::range)>(matrix, index);

	auto R = row_sentinel(matrix, index);
	auto i = find(R, [](auto&& x) { return x != 0; });
	view_base_t<MatrixView> gcd = 0;
	if (i != ::end(R))
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
template <flag::bitmask flags = flag::none, typename MatrixView, typename T>
constexpr void add_rows(MatrixView& matrix, std::size_t i1, const T& scalar, std::size_t i2)
{
	detail::check_row_indices<flag_set(flags, flag::range)>(matrix, i1, i2);

	zip(row_sentinel(matrix, i2), row_sentinel(matrix, i1),
		[&scalar](auto&& x, auto&& y) { x += scalar * y; });
}

template <typename ResizableMatrixView>
constexpr void resize(ResizableMatrixView& matrix, std::size_t size, std::size_t width)
{
	matrix.resize(size, width);
}
template <typename ResizableMatrixView>
void append_rows(ResizableMatrixView& matrix, std::size_t count)
{
	resize(matrix, size(matrix) + count * width(matrix), width(matrix));
}
template <typename ResizableMatrixView>
void append_columns(ResizableMatrixView& matrix, std::size_t count)
{
	auto old_width = width(matrix);

	resize(matrix, size(matrix) + count * height(matrix), width(matrix) + count);

	for (std::size_t i = height(matrix) - 1; i != 0; --i)
		for (std::size_t j = width(matrix) - 1; j != -1; --j)
			element(matrix, i, j) = element(matrix, i, j, old_width);
}

template <bool reduced = false, bool calculate_determinant = false, bool calculate_rank = false, typename MatrixView>
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
template <typename MatrixView>
constexpr auto REF_det(MatrixView& matrix)
{
	return REF<false, true, false>(matrix);
}
template <typename MatrixView>
constexpr auto rank(MatrixView& matrix)
{
	return REF<false, false, true>(matrix);
}
template <typename MatrixView>
constexpr auto REF_reduced_rank(MatrixView& matrix)
{
	return REF<true, false, true>(matrix);
}
template <flag::bitmask flags = flag::none, typename MatrixView>
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
template <flag::bitmask flags = flag::none, typename MatrixView>
constexpr auto determinant(MatrixView& matrix)
{
	if constexpr (flag_set(flags, flag::triangular))
		return determinant<flags>(matrix, true);
	else
		return determinant<flags>(matrix, is_triangular(matrix));
}
template <flag::bitmask flags = flag::none, typename MatrixView, typename MatrixViewVector>
constexpr std::make_signed_t<std::size_t> solve_linear_equations(MatrixView& m, MatrixViewVector& vector)
{
	if constexpr (flag_set(flags, flag::height))
		if (size(vector) != height(matrix))
			throw "invalid size of vector";

	auto w = width(m);
	augmented_matrix_view matrix(m, vector);

	auto r = REF_reduced_rank(matrix);

	std::size_t sub_rank = 0, j = 0;
	while (sub_rank != height(matrix) && j != w)
	{
		if (element(matrix, sub_rank, j) != 0)
			++sub_rank;
		++j;
	}

	if (sub_rank < r)
		return -1;
	else
	{
		if (r == w)
		{
			auto v = begin(vector);
			for (std::size_t i = 0; v != end(vector); ++i, ++v)
				*v = element(matrix, i, w) / element(matrix, i, i);
		}
		return w - r;
	}
}

namespace detail
{
	template <typename MatrixView, typename Printer>
	std::ostream& print_helper(std::ostream& out, const MatrixView& matrix, Printer printer)
	{
		for (auto&& row : rows_sentinel(matrix))
		{
			out << '|';
			auto i = begin(row);
			auto a = end(row);
			auto b = ::prev(a);
			for (; i != b; ++i)
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

template <typename MatrixView>
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

template <typename MatrixView>
std::ostream& basic_print(std::ostream& out, const MatrixView& matrix)
{
	return detail::print_helper(out, matrix,
		[&out](const auto& element)
		{
			out << element;
		});
}
