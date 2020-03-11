#include <iostream>
#include <array>
#include "augmented_matrix_view.hpp"
#include "dynamic_matrix.hpp"
#include "static_matrix.hpp"
#include "operations.hpp"
#include "equal.hpp"
#include "take_view.hpp"

int main()
{
	PPmatrix::dynamic_matrix<int> A(2, 2);
	PPmatrix::copy({ 1, 2, 1, 3 }, A);

	PPmatrix::transpose(A);
	PPmatrix::pretty_print(std::cout, A);
	PPmatrix::transpose(A);

	PPmatrix::static_matrix<int, 2> b(1);
	PPmatrix::copy({ 10, 13 }, b);

	PPmatrix::pretty_print(std::cout, PPmatrix::augmented_matrix_view(A, b));

	std::cout << "dimensions: " << PPmatrix::solve_linear_equations(A, b) << std::endl;

	PPmatrix::pretty_print(std::cout, PPmatrix::augmented_matrix_view(A, b));

	PPmatrix::pretty_print(std::cout, b);

	PPmatrix::static_matrix<PPmatrix::Q, 36> C(6);
	PPmatrix::copy({
		1, 5, 9, 0, 4, 0,
		9, 6, 7, 3, 6, 8,
		0, 8, 3, 2, 4, 7,
		4, 0, 1, 5, 6, 1,
		7, 8, 7, 6, 3, 9,
		0, 3, 9, 7, 3, 2 }, C);

	std::cout << PPmatrix::determinant(C) << std::endl;

	return 0;
}
