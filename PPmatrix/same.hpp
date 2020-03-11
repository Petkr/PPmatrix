#pragma once

namespace PPmatrix
{
	namespace detail
	{
		template <typename T, typename U>
		inline constexpr bool same = false;

		template <typename T>
		inline constexpr bool same<T, T> = true;
	}

	template <typename T, typename U>
	concept same = detail::same<T, U>;
}
