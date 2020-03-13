#pragma once
#include "compressed_pair.hpp"
#include "matrix_view.hpp"
#include "PP.hpp"

namespace PPmatrix
{
	template <iterator Iterator, sentinel<Iterator> Sentinel>
	class simple_matrix_view
	{
		size_t width_;
		compressed_pair<Iterator, Sentinel> pair;

	public:
		constexpr simple_matrix_view(Iterator begin, Sentinel end, size_t width)
			: width_(width)
			, pair{ begin, end }
		{}
		constexpr simple_matrix_view(view auto&& v, size_t width)
			: simple_matrix_view(PP::begin(v), PP::end(v), width)
		{}
		constexpr simple_matrix_view(matrix_view auto&& v)
			: simple_matrix_view(v, PP::width(v))
		{}
		constexpr iterator auto begin() const
		{
			return pair.first;
		}
		constexpr auto end() const
		{
			return pair.second;
		}
		constexpr size_t width() const
		{
			return width_;
		}

		constexpr void set_width(size_t width)
		{
			width_ = width;
		}
	};
	template <matrix_view MatrixView>
	simple_matrix_view(MatrixView&&) -> simple_matrix_view<begin_t<MatrixView>, end_t<MatrixView>>;
	template <view View>
	simple_matrix_view(View&&, size_t) -> simple_matrix_view<begin_t<View>, end_t<View>>;
}
