#pragma once
#include "view.hpp"

namespace PPmatrix
{
	namespace detail
	{
		template <typename T>
		concept has_width = requires (const T t)
		{
			t.width();
		};
	}
	template <typename MatrixView>
	constexpr std::size_t width(const MatrixView& matrix)
	{
		if constexpr (detail::has_width<MatrixView>)
			return matrix.width();
		else
			return 1;
	}
	template <typename MatrixView>
	constexpr std::size_t height(const MatrixView& matrix)
	{
		return PPmatrix::size(matrix) / width(matrix);
	}
}
