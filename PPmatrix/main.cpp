#include <iostream>
#include <array>
#include "augmented_matrix_view.hpp"
#include "dynamic_matrix.hpp"
#include "static_matrix.hpp"
#include "matrix_view.hpp"

constexpr auto print = [](auto&& x)
{
	std::cout << std::boolalpha << x << '\n';
};

template <typename MatrixView>
void matrix_print(const MatrixView& M)
{
	pretty_print(std::cout, M);
}

int main()
{
	dynamic_matrix<int> A(2, 2);
	copy(std::initializer_list<int>{ 1, 2, 1, 3 }, A);

	static_matrix<int, 2> b(1);
	copy(std::initializer_list<int>{ 10, 13 }, b);

	matrix_print(augmented_matrix_view(A, b));

	solve_linear_equations(A, b);

	matrix_print(b);

	return 0;
}
