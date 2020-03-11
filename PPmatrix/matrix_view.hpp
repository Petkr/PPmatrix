#pragma once
#include "view.hpp"

namespace PPmatrix
{
	namespace detail
	{
		template <typename T>
		concept view_with_width =
			view<T> &&
			requires (const T t)
			{
				{ t.width() } -> is_size_t;
				// { t.width() } -> same<std::size_t>; // doesn't compile
			};
	}

	consteval std::size_t width(view auto&&)
	{
		return 1;
	}
	constexpr std::size_t width(detail::view_with_width auto&& matrix)
	{
		return matrix.width();
	}

	template <typename MatrixView>
	concept matrix_view =
		view<MatrixView> &&
		requires (const MatrixView mv)
		{
			{ width(mv) } -> same<std::size_t>;
		};

	constexpr std::size_t height(matrix_view auto&& matrix)
	{
		return PPmatrix::size(matrix) / width(matrix);
	}
}
