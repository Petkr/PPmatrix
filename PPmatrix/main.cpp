#include <iostream>
#include <array>
#include "augmented_matrix_view.hpp"
#include "dynamic_matrix.hpp"
#include "static_matrix.hpp"
#include "operations.hpp"

int main()
{
	PPmatrix::dynamic_matrix<int> A(2, 2);
	PPmatrix::copy({ 1, 2, 1, 3 }, A);

	PPmatrix::static_matrix<int, 2> b(1);
	PPmatrix::copy({ 10, 13 }, b);

	PPmatrix::pretty_print(std::cout, PPmatrix::augmented_matrix_view(A, b));

	PPmatrix::solve_linear_equations(A, b);

	PPmatrix::pretty_print(std::cout, b);

	return 0;
}
