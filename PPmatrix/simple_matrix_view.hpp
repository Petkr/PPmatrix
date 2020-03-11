#pragma once
#include "compressed_pair.hpp"
#include "matrix_view.hpp"

namespace PPmatrix
{
	template <iterator Iterator, sentinel<Iterator> Sentinel>
	class simple_matrix_view
	{
		std::size_t width_;
		compressed_pair<Iterator, Sentinel> pair;

	public:
		constexpr simple_matrix_view(view auto&& v, std::size_t width)
			: width_(width)
			, pair{ PPmatrix::begin(v), PPmatrix::end(v) }
		{}
		constexpr simple_matrix_view(matrix_view auto&& v)
			: simple_matrix_view(v, PPmatrix::width(v))
		{}
		constexpr simple_matrix_view(Iterator begin, Sentinel end, std::size_t width)
			: width_(width)
			, pair{ begin, end }
		{}
		constexpr iterator auto begin() const
		{
			return pair.first;
		}
		constexpr iterator auto end() const
		{
			return pair.second;
		}
		constexpr std::size_t width() const
		{
			return width_;
		}

		constexpr void set_width(std::size_t width)
		{
			width_ = width;
		}
	};
	template <matrix_view MatrixView>
	simple_matrix_view(MatrixView&&) -> simple_matrix_view<begin_t<MatrixView>, end_t<MatrixView>>;
	template <view View>
	simple_matrix_view(View&&, std::size_t) -> simple_matrix_view<begin_t<View>, end_t<View>>;
}
