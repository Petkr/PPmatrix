#pragma once

namespace detail
{
	template <typename View>
	constexpr void execute(View&& view)
	{
		auto b = begin(view);
		auto e = end(view);
		for (auto i = b; i != e; ++i)
			(void)*i;
	}
}

struct execute_t {};
constexpr execute_t execute{};

template <typename View>
constexpr auto operator|(View&& view, execute_t)
{
	detail::execute(view);
}
