cmake_minimum_required (VERSION 3.8)

file(GLOB MAIN_SOURCES *.cpp)

add_executable(PPmatrix ${MAIN_SOURCES})

target_compile_features(PPmatrix PUBLIC cxx_std_20)
if ( CMAKE_COMPILER_IS_GNUCXX )
    target_compile_options(PPmatrix PRIVATE -Wall)
    target_compile_options(PPmatrix PRIVATE -Wextra)
    target_compile_options(PPmatrix PRIVATE -Werror)
    target_compile_options(PPmatrix PRIVATE -fconcepts)
    target_compile_options(PPmatrix PRIVATE -ftemplate-backtrace-limit=0)
endif()
if ( MSVC )
    target_compile_options(PPmatrix PRIVATE /W4)
    target_compile_options(PPmatrix PRIVATE /WX)
endif()
