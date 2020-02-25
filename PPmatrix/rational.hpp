#pragma once
#include <numeric>
#include <type_traits>
#include <cstdint>
#include <iostream>

struct dont_simplify_tag_t {};
constexpr dont_simplify_tag_t dont_simplify_tag{};

template <typename T>
struct rational
{
	static_assert(std::is_integral_v<T>);

	T num;
	T den;

	constexpr rational(T numerator, T denominator, dont_simplify_tag_t)
		: num(numerator)
		, den(denominator)
	{}
	constexpr rational(T numerator, T denominator)
		: rational(numerator, denominator, dont_simplify_tag)
	{
		simplify();
	}
	constexpr rational(T number)
		: rational(number, 1, dont_simplify_tag)
	{}
	constexpr rational()
		: rational(T())
	{}
private:
	constexpr void simplify()
	{
		auto gcd = std::gcd(num, den);
		num /= gcd;
		den /= gcd;

		if (den < 0)
		{
			num = -num;
			den = -den;
		}
	}
public:
	constexpr auto& operator=(T other)
	{
		num = other;
		den = 1;
		return *this;
	}
	constexpr auto& operator+=(rational other)
	{
		auto lcm = std::lcm(den, other.den);
		num = (lcm / den) * num + (lcm / other.den) * other.num;
		den = lcm;
		simplify();
		return *this;
	}
	constexpr auto& operator+=(T other)
	{
		num += other * den;
		return *this;
	}
	constexpr auto& operator-=(rational other)
	{
		auto lcm = std::lcm(den, other.den);
		num = (lcm / den) * num - (lcm / other.den) * other.num;
		den = lcm;
		simplify();
		return *this;
	}
	constexpr auto& operator-=(T other)
	{
		num -= other * den;
		return *this;
	}
	constexpr auto& operator*=(rational other)
	{
		num *= other.num;
		den *= other.den;
		simplify();
		return *this;
	}
	constexpr void invert()
	{
		std::swap(num, den);
	}
	constexpr auto inverse() const
	{
		return rational(den, num, dont_simplify_tag);
	}
	constexpr auto& operator/=(rational other)
	{
		return *this *= other.inverse();
	}
	constexpr bool is_integral() const
	{
		return den == 1;
	}
	constexpr bool is_zero() const
	{
		return num == 0;
	}
	constexpr bool operator==(T other) const
	{
		return is_integral() && num == other;
	}
	constexpr bool is_less_than_one() const
	{
		return num < den;
	}
	constexpr bool is_negative() const
	{
		return num < 0;
	}
	constexpr bool is_positive() const
	{
		return num > 0;
	}
	constexpr bool operator<(rational other) const
	{
		auto lcm = std::lcm(den, other.den);
		return num * (lcm / den) < other.num * (lcm / other.den);
	}
	constexpr bool operator<(T other) const
	{
		return num < other * den;
	}
	constexpr void negate()
	{
		num = -num;
	}
	constexpr auto operator-() const
	{
		return rational(-num, den, dont_simplify_tag);
	}
	constexpr auto operator!=(rational other) const
	{
		return num != other.num || den != other.den;
	}
	explicit constexpr operator T() const
	{
		return num / den;
	}
};

template <typename T>
constexpr auto number_width(rational<T> q)
{
	std::size_t width = number_width(q.num);
	if (!q.is_integral())
		width += 1 + number_width(q.den);
	return width;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, rational<T> q)
{
	out << q.num;
	if (!q.is_integral())
		out << '/' << q.den;
	return out;
}

using Q = rational<int>;
