#pragma once
#include "same.hpp"

namespace PPmatrix
{
	template <typename T>
	concept nonvoid = !same<T, void>;
}
