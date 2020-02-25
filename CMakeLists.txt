cmake_minimum_required (VERSION 3.8)

project (PPmatrix)
add_subdirectory (PPmatrix)
target_compile_definitions(PPmatrix PRIVATE _ITERATOR_DEBUG_LEVEL=0)
