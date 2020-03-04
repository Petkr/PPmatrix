#pragma once

namespace PPmatrix::functor
{
	constexpr auto product		= [](auto&& x, auto&& y)	{ return x * y; };
	constexpr auto sum			= [](auto&& x, auto&& y)	{ return x + y; };
	constexpr auto add			= [](auto&& x, auto&& y)	{ return x += y; };
	constexpr auto dereference	= [](auto&& x)				{ return *x; };
}