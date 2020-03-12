#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "PPmatrix.hpp"

int main()
{
	PPmatrix::copy(
		{ 5, 4, 7, 3, 6 , 8, 4 },
		std::ostream_iterator<int>(std::cout, " wow ") ^ PPmatrix::unbounded);
	
	return 0;
}
