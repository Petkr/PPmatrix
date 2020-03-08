#pragma once
#include "simple_view.hpp"

namespace PPmatrix
{
	template <iterator BaseIterator, typename Transform>
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
		constexpr auto& operator-=(std::size_t offset)
		{
			pair.first -= offset;
			return *this;
		}
		template <iterator OtherIterator>
		constexpr bool operator!=(OtherIterator other) const
		{
			return compare_iterator(pair.first, other);
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

	template <view View, typename Functor>
	constexpr view transform_view(View&& v, Functor f)
	{
		return transform_iterator(begin(v), f) ^ transform_iterator(end(v), f);
	}

	template <iterator Iterator, typename Functor>
	constexpr iterator operator&(Iterator i, transform<Functor> t)
	{
		return transform_iterator(i, t.functor);
	}

	template <view View, typename Functor>
	constexpr view operator||(View&& v, transform<Functor> t)
	{
		return begin(v) & t ^ end(v);
	}

	template <view View, typename Functor>
	constexpr view operator|(View&& v, transform<Functor> t)
	{
		return transform_view(std::forward<View>(v), t.functor);
	}
}
