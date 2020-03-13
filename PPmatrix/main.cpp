#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "PPmatrix.hpp"

int main()
{
	std::array<int, 5> arr;

	[[maybe_unused]]
	auto v = PPmatrix::simple_view(arr);

	PPmatrix::dynamic_matrix<int> A(2, 2);
	PPmatrix::copy({ 1, 2, 1, 3 }, A);

	PPmatrix::static_matrix<int, 2> b(1);
	PPmatrix::copy({ 10, 13 }, b);

	PPmatrix::pretty_print(std::cout, PPmatrix::augmented_matrix_view(A, b));

	PPmatrix::solve_linear_equations(A, b);

	PPmatrix::pretty_print(std::cout, b);

	return 0;
}
