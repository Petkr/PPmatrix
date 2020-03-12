#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "PPmatrix.hpp"

int main()
{
	int width, height;

	std::cin >> width >> height;

	PPmatrix::dynamic_matrix<int> matrix(height, width);

	for (auto&& column : PPmatrix::columns_sentinel(matrix))
		for (auto&& element : column)
			std::cin >> element;

	PPmatrix::pretty_print(std::cout, matrix);

	return 0;
}
