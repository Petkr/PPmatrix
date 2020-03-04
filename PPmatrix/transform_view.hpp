#pragma once
#include "simple_view.hpp"

namespace PPmatrix
{
	template <typename BaseIterator, typename Transform>
	class transform_iterator
	{
		compressed_pair<BaseIterator, Transform> pair;
	public:
		constexpr transform_iterator(BaseIterator base_iterator, Transform t)
			: pair{ base_iterator, t }
		{}
		constexpr decltype(auto) operator*()
		{
			return pair.second(*pair.first);
		}
		constexpr auto& operator+=(std::size_t offset)
		{
			pair.first += offset;
			return *this;
		}
		template <typename OtherIterator>
		constexpr auto operator!=(OtherIterator other_iterator) const
		{
			return other_iterator != pair.first;
		}
	};

	template <typename Functor>
	struct transform
	{
		Functor functor;
		constexpr transform(Functor functor)
			: functor(functor)
		{}
	};

	template <typename Iterator, typename Functor>
	constexpr auto operator&(Iterator i, transform<Functor> t)
	{
		return transform_iterator(i, t.functor);
	}

	template <typename View, typename Functor>
	constexpr auto operator||(View&& view, transform<Functor> t)
	{
		return begin(view) & t ^ end(view);
	}

	template <typename View, typename Functor>
	constexpr auto operator|(View&& view, transform<Functor> t)
	{
		return (begin(view) & t) ^ (end(view) & t);
	}
}
