#pragma once

namespace PPmatrix::functor
{
	// just for shorter code when using PPmatrix::transform

	inline constexpr auto product		= [](auto&& x, auto&& y)	{ return x * y; };
	inline constexpr auto sum			= [](auto&& x, auto&& y)	{ return x + y; };
	inline constexpr auto add			= [](auto&& x, auto&& y)	{ return x += y; };
	inline constexpr auto dereference	= [](auto&& x)				{ return *x; };
}