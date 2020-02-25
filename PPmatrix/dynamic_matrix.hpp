#pragma once
#include <cstddef>
#include <vector>
#include "view.hpp"

template <typename T>
class dynamic_matrix
{
	std::size_t width_;
	std::vector<T> data_;

public:
	dynamic_matrix(std::size_t height, std::size_t width)
		: width_(width)
		, data_(width_ * height)
	{}

	auto begin()
	{
		return ::begin(data_);
	}
	auto begin() const
	{
		return ::begin(data_);
	}
	auto end()
	{
		return ::end(data_);
	}
	auto end() const
	{
		return ::end(data_);
	}
	auto width() const
	{
		return width_;
	}

	void resize(std::size_t size, std::size_t width)
	{
		data_.resize(size);
		width_ = width;
	}
};
