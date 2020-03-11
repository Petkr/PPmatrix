# PPmatrix

Referencia zápočtového programu PPmatrix.

# Obsah

* [Koncepty](#Koncepty)
  * [same](#same)
  * [nonvoid](#nonvoid)
  * [iterator](#iterator)
  * [view](#view)
  * [matrix_view](#matrix_view)
* [Maticové operácie](#maticové-operácie)
  * [multiply](#multiply)
  * [add](#add)
  * [transpose](#transpose)
  * [determinant](#determinant)
  * [solve_linear_equations](#solve_linear_equations)
  * [REF](#REF)
* [Maticové containery](#maticové-containery)
  * [static_matrix](#static_matrix)
  * [dynamic_matrix](#dynamic_matrix)
* [Views](#Views)
  * [simple_view](#simple_view)
  * [simple_matrix_view](#simple_matrix_view)
  * [augmented_matrix_view](#augmented_matrix_view)
* [Generické algoritmy](#generické-algoritmy)
  * [accumulate](#accumulate)
  * [copy](#copy)
  * [equal](#equal)
  * [fill](#fill)
  * [find](#find)
  * [inner_product](#inner_product)
  * [max_element](#max_element)
  * [swap_ranges](#swap_ranges)
  * [zip](#zip)
* [Iterator adaptory](#iterator-adaptory)
  * [skip_iterator](#skip_iterator)
  * [transform_iterator](#transform_iterator)
  * [static_iterator](#static_iterator)
  * [wrap_iterator](#wrap_iterator)
  * [take_iterator](#take_iterator)
* [Ostatné](#Ostatné)
  * [unbounded](#unbounded)
  * [rational](#rational)

## Koncepty

### same

```cpp
template <typename T, typename U>
concept same = detail::same<T, U>;
```

Splnený, ak sú typy zhodné.

### nonvoid

```cpp
template <typename T>
concept nonvoid = !same<T, void>;
```

### iterator

```cpp
template <typename Iterator>
concept iterator = requires (Iterator i)
{
	{ ++i } -> same<Iterator&>;
	{ *i } -> nonvoid;
};
```

### sentinel

```cpp
template <typename Sentinel, typename Iterator>
concept sentinel =
	iterator<Iterator> &&
	requires (const Iterator i, const Sentinel s)
	{
		{ i == s } -> same<bool>;
	};
```

### view

```cpp
template <typename View>
concept view = requires (View v)
{
	{ begin(v) } -> iterator;
	{ end(v) } -> sentinel<decltype(begin(v))>;
};
```

### matrix_view

```cpp
template <typename MatrixView>
concept matrix_view =
	view<MatrixView> &&
	requires (const MatrixView mv)
	{
		{ width(mv) } -> same<size_t>;
	};
```

## Maticové operácie

### multiply

(1)

```cpp
template <size_t flags = flag::none>
constexpr void multiply(matrix_view auto&& A, matrix_view auto&& B, matrix_view auto&& result);
```

Násobí maticu `A` maticou `B`. Výsledok uloží do matice `result`.

`flags::size` zapne kontrolu správnych rozmerov matíc.

(2)

```cpp
constexpr void multiply(matrix_view auto&& matrix, const auto& scalar);
```

Násobí maticu `matrix` skalárom `scalar`.

### add

```cpp
template <size_t flags = flag::none>
constexpr auto& add(matrix_view auto&& A, matrix_view auto&& B);
```

Pripočíta k matici `A` maticu `B`. Vráti `A` po pričítaní.

`flags::size` zapne kontrolu správnych rozmerov matíc.

### transpose

(1)

```cpp
template <bool const_square = false>
constexpr void transpose(matrix_view auto&& matrix, bool square);
```

Transponuje maticu `matrix`.

Ak je `const_square` alebo `square` `true`,
transponuje, akoby bola `matrix` štvorcová matica.\
Inak použije
[všeobecný algoritmus](https://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose).

`transpose<true>(matrix, b)` ignoruje `b`.

(2)

```cpp
template <bool const_square = false>
constexpr void transpose(matrix_view auto&& matrix);
```

Transponuje maticu `matrix`.

`transpose<true>(matrix)` volá `transpose<true>(matrix, _)`.\
`transpose<false>(matrix)` volá `transpose<true>(matrix, is_square(matrix))`.

### determinant

(1)

```cpp
template <size_t flags = flag::none>
constexpr auto determinant(matrix_view auto&& matrix, bool triangular);
```

Vráti determinant matice `matrix`.

`flags::square` zapne kontrolu, či je `matrix` štvorcová matica.

Ak je nastavené `flags::triangular` alebo `triangular` `true`,
spočíta determinant, akoby bola `matrix` trojuholníková matica.\
Inak použije všeobecný algoritmus.

Ak má `flags` nastavené `flags::triangular`, `determinant<flags>(matrix, b)` ignoruje `b`.

(2)

```cpp
template <flag::bitmask flags = flag::none>
constexpr auto determinant(matrix_view auto&& matrix);
```

Vráti determinant matice `matrix`.

`flags::square` ako pri (1).

Ak je nastavené `flags::triangular`, volá `determinant<flags>(matrix, _)`.\
Inak volá `determinant<flags>(matrix, is_triangular(matrix))`.

### solve_linear_equations

```cpp
template <flag::bitmask flags = flag::none>
constexpr std::make_signed<size_t> solve_linear_equations(matrix_view auto&& M, matrix_view auto&& v);
```

Pre rovnicu `Mb = v`:

Ak neexistuje `b`, vráti -1.\
Ak existuje, vráti počet dimenzií riešenia.

Ak existuje práve jedno riešenie, uloží ho do `v`, inak nechá prvky `v` v neurčenom stave.

`flag::height` zapne kontrolu rozmerov matíc.

### REF

```cpp
template <
	bool reduced = false,
	bool calculate_determinant = false,
	bool calculate_rank = false>
constexpr auto REF(matrix_view auto&& matrix);
```

Upraví maticu `matrix` do (R)REF.

`REF<false, d, r>` upraví do REF.\
`REF<true, d, r>` upraví do RREF.

`REF<rr, false, false>` má návratový typ `void`.\
`REF<rr, true, false>` vracia determinant matice `matrix`.\
`REF<rr, false, true>` vracia rank matice `matrix`.\
`REF<rr, true, true>` vracia determinant a rank cez `std::pair`.

## Maticové containery

Základné containery, ktoré spĺňajú `matrix_view`.

### static_matrix

```cpp
template <
	typename T,
	size_t size>
class static_matrix
{
	constexpr explicit static_matrix(size_t width);

	constexpr iterator auto begin();
	constexpr iterator auto begin() const;
	constexpr iterator auto end();
	constexpr iterator auto end() const;
	constexpr size_t width() const;
};
```

Container so statickou veľkosťou spĺňajúci `matrix_view`.

### dynamic_matrix

```cpp
template <typename T>
class dynamic_matrix
{
	dynamic_matrix(size_t height, size_t width);

	iterator auto begin();
	iterator auto begin() const;
	iterator auto end();
	iterator auto end() const;
	size_t width() const;
	void resize(size_t new_height);
	void resize(size_t new_width, resize_columns_tag_t);
};
```

Container s dynamickou veľkosťou spĺňajúci `matrix_view`.

## Views

### simple_view

```cpp
	template <
		iterator Iterator,
		sentinel<Iterator> Sentinel>
	class simple_view
	{
	public:
		constexpr simple_view(view auto&& v);
		constexpr simple_view(Iterator begin, Sentinel end);

		constexpr iterator auto begin() const;
		constexpr iterator auto end() const;
	};
```

Ukladá dvojicu iteratorov begin a end. Spĺňa `view`.

### simple_matrix_view

```cpp
template <
	iterator Iterator,
	sentinel<Iterator> Sentinel>
class simple_matrix_view
{
public:
	constexpr simple_matrix_view(view auto&& v, size_t width);
	constexpr simple_matrix_view(matrix_view auto&& v);
	constexpr simple_matrix_view(Iterator begin, Sentinel end, size_t width);
	constexpr iterator begin() const;
	constexpr iterator end() const;
	constexpr size_t width() const;
	constexpr void set_width(size_t width);
};
```

Ukladá dvojicu iteratorov begin a end a šírku matice. Spĺňa `matrix_view`.

### augmented_matrix_view

```cpp
template <
	typename LeftMatrixView,
	typename RightMatrixView>
class augmented_matrix_view
{
	augmented_matrix_view(matrix_view auto&& left, matrix_view auto&& right);
	augmented_matrix_view(matrix_view auto&& left, matrix_view auto&& right, dont_check_heights_tag_t);

	iterator auto begin() const;
	iterator auto end() const;
	size_t width() const;
	size_t size() const;
};
```

Reprezentuje
[rozšírenú maticu sústavy](https://en.wikipedia.org/wiki/Augmented_matrix)
(A | B).

Spĺňa `matrix_view`.

## Generické algoritmy

Keďže výraz `{1, 2, 3}` nemá v C++ typ, kompilátor nedokáže vydedukovať template parameter, napríklad pri volaní `accumulate({1, 2, 3}, [](auto&& x, auto&& y){ return x + y; })`.

Algoritmy, ktoré akceptujú `view` na konštantné prvky majú teda navyše overloady pre `std::initializer_list`.

### accumulate

(1)

```cpp
template <
	view View,
	typename T = view_base_t<View>>
constexpr T accumulate(View&& v, auto f, T init = {});
```

Vráti ľavý fold `v` cez binárnu funkciu `f` s počiatočnou hodnotou `init`.

Napríklad pre `v` s prvkami `{ 1, 2, 5, 9 }`, funkciu `f` a `init` rovný `0`
vráti `f(f(f(f(init, 1), 2), 5), 9)`.

(2)

```cpp
template <
	typename T,
	typename U = T>
constexpr U accumulate(const std::initializer_list<T>& l, auto f, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### copy

(1)

```cpp
constexpr void copy(view auto&& from, view auto&& to);
```

Nakopíruje `from` do `to` po prvkoch pomocou `operator=`.

Počet volaní `operator=` je rovný `std::min(PPmatrix::size(from), PPmatrix::size(to))`.

(2)

```cpp
constexpr void copy(const std::initializer_list<auto>& from, view auto&& to);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### equal

(1)

```cpp
constexpr bool equal(view auto&& view1, view auto&& view2);
```

Určí, či sú `view1` a `view2` zhodné po prvkoch podľa `operator==`.

Z oboch `view` porovnáva iba prvých `std::min(PPmatrix::size(view1), PPmatrix::size(view2))` prvkov.

(2)

```cpp
constexpr bool equal(const std::initializer_list<auto>& l, view auto&& view2);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```cpp
constexpr bool equal(view auto&& view1, const std::initializer_list<auto>& l);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```cpp
constexpr bool equal(const std::initializer_list<auto>& l, const std::initializer_list<auto>& m);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### fill

```cpp
constexpr void fill(view auto&& view, const auto& value);
```

### find

(1)

```cpp
constexpr iterator auto find(view auto&& v, auto p);
```

Ak existuje element `x` vo `v`, pre ktorý `p(x)` vráti `true`, vráti iterator na prvý taký.\
Inak vráti iterator rovný `PPmatrix::end(view)`.

(2)

```cpp
constexpr iterator auto find(const std::initializer_list<auto>& l, auto p);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### inner_product

(1)

```cpp
template <
	view View1,
	view View2,
	typename T = detail::accumulator_t<View1, View2>>
constexpr T inner_product(View1&& view1, View2&& view2, T init = {});
```

Vráti súčet `init` a súčinov prvkov `view1` a `view2`.

(2)

```cpp
template <
	typename T,
	view View2,
	typename U = detail::accumulator_t<std::initializer_list<T>, View2>>
constexpr U inner_product(const std::initializer_list<T>& l, View2&& view2, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```cpp
template <
	view View1,
	typename T,
	typename U = detail::accumulator_t<View1, std::initializer_list<T>>>
constexpr U inner_product(View1&& view1, const std::initializer_list<T>& l, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```cpp
template <
	typename T,
	typename U,
	typename V = detail::accumulator_t<std::initializer_list<T>, std::initializer_list<U>>>
constexpr V inner_product(const std::initializer_list<T>& l, const std::initializer_list<U>& m, V init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### max_element

(1)

```cpp
constexpr iterator max_element(view auto&& v);
```

Vráti iterator na najväčší prvok vo `v`.

(2)

```cpp
constexpr iterator max_element(const std::initializer_list<auto>& l);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### swap_ranges

```cpp
constexpr void swap_ranges(view auto&& view1, view auto&& view2);
```

Prehodí prvky `view1` a `view2` použitím `std::swap`.

### zip

(1)

```cpp
constexpr void zip(view auto&& v, view auto&& w, auto f);
```

Zavolá `f` na jednotlivé prvky `v` a `w`. Teda pre prvky `a` a `b` zavolá `f(a, b)`.

Počet volaní `f` je rovný `std::min(PPmatrix::size(v), PPmatrix::size(w))`.

(2)

```cpp
constexpr void zip(const std::initializer_list<auto>& l, view auto&& w, auto f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```cpp
constexpr void zip(view auto&& v, const std::initializer_list<auto>& l, auto f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```cpp
constexpr void zip(const std::initializer_list<auto>& l, const std::initializer_list<auto>& m, auto f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

## Iterator adaptory

### skip_iterator

```cpp
template <iterator BaseIterator>
class skip_iterator
{
public:
	constexpr skip_iterator(BaseIterator base_iterator, size_t skip_length);

	constexpr decltype(auto) operator*() const;
	constexpr auto& operator+=(size_t offset);
	constexpr auto& operator-=(size_t offset);
	constexpr bool operator==(iterator auto other) const;
	constexpr auto& base();
};

struct skip
{
	skip(size_t skip_length);
};
```

Pre `skip_iterator si(i, sl)`:\
`si += n` volá `i += n * sl`\
`*si` vracia `*i`.

### transform_iterator

```cpp
template <
	iterator BaseIterator,
	typename Transform>
class transform_iterator
{
public:
	constexpr transform_iterator(BaseIterator base_iterator, Transform t);

	constexpr decltype(auto) operator*() const;
	constexpr auto& operator+=(size_t offset);
	constexpr auto& operator-=(size_t offset);
	constexpr bool operator==(iterator auto other) const;
};

template <typename Functor>
struct transform
{
	constexpr transform(Functor functor);
};
```

Pre `transform_iterator ti(i, f)`:\
`ti += n` volá `i += n`\
`*ti` vracia `f(*i)`.

### static_iterator

```cpp
template <typename T>
class static_iterator
{
public:
	constexpr static_iterator(T&& value);
	constexpr static_iterator(const T& value);

	constexpr const auto& operator*() const;
	constexpr auto& operator+=(size_t);
	constexpr auto& operator-=(size_t);
	constexpr bool operator==(const static_iterator<auto>& other) const
};
```

Iterator, ktorý simuluje nekonečný `view` s rovnakými prvkami.\
Jeho `operator+=` nerobí nič a `operator*` vracia obalený objekt.

##### static_view

```cpp
template <typename T>
constexpr view auto static_view(T&& value)
```

Wrapper, ktorý vracia object spĺňajúci `view`.

### wrap_iterator

```cpp
template <typename T>
class wrap_iterator
{
	T value;
public:
	constexpr wrap_iterator(const T& value);
	constexpr wrap_iterator(T&& value);

	constexpr auto& operator*();
	constexpr const auto& operator*() const;
	constexpr auto& operator+=(size_t offset);
	constexpr auto& operator-=(size_t offset);
	constexpr bool operator==(const T& other) const;
	constexpr bool operator==(const wrap_iterator<auto>& other) const;
};
```

Pre `wrap_iterator wi(x)`:\
`wi += n` volá `x += n`\
`*wi` vracia `x`.

##### wrap_view

(1)

```cpp
constexpr view auto wrap_view(auto&& begin, auto&& end);
```

Vracia [`view`](#view), ktorý má begin a end typu [`wrap_iterator`](#wrap_iterator).

(2)

```cpp
constexpr view auto wrap_view(auto&& begin);
```

Vracia [`unbounded`](#unbounded) [`view`](#view), ktorý má begin typu
[`wrap_iterator`](#wrap_iterator).

### take_iterator

```cpp
template <iterator BaseIterator>
class take_iterator
{
public:
	constexpr take_iterator(BaseIterator base_iterator, size_t count);

	constexpr decltype(auto) operator*() const;
	constexpr auto& operator+=(size_t offset);
	constexpr auto& operator-=(size_t offset);
	constexpr bool operator==(iterator auto i) const;
};
```

Iterator, ktorý sám simuluje `view`, ktorý si namiesto end pamätá svoju veľkosť.

##### take

```cpp
struct take
{
	constexpr take(size_t count);
};
```

Pozri [použitie]().

##### take_view

```cpp
constexpr view auto take_view(iterator auto i, size_t n);
```

Wrapper, ktorý vracia object spĺňajúci `view`.

## Ostatné

### unbounded

```cpp
struct unbounded_t {};
inline constexpr unbounded_t unbounded{};
```

Pre každý iterator `i` vráti `i != unbounded` `false`.

[`view`](#view), ktorý má `unbounded` ako svoj end, je nekonečný.

### rational

```cpp
struct dont_simplify_tag_t {};
constexpr dont_simplify_tag_t dont_simplify_tag{};

template <typename T>
struct rational
{
	static_assert(std::is_integral_v<T>);

	constexpr rational(T numerator, T denominator, dont_simplify_tag_t);
	constexpr rational(T numerator, T denominator);
	constexpr rational(T number);
	constexpr rational();

	constexpr auto& operator=(T other);
	constexpr auto& operator+=(rational other);
	constexpr auto& operator+=(T other);
	constexpr auto& operator-=(rational other);
	constexpr auto& operator-=(T other);
	constexpr auto& operator*=(rational other);
	constexpr auto operator*(rational other) const;
	constexpr void invert();
	constexpr auto inverse() const;
	constexpr auto& operator/=(rational other);
	constexpr auto operator/(rational other) const;
	constexpr bool is_integral() const;
	constexpr bool is_zero() const;
	constexpr bool operator==(T other) const;
	constexpr bool is_less_than_one() const;
	constexpr bool is_negative() const;
	constexpr bool is_positive() const;
	constexpr bool operator<(rational other) const;
	constexpr bool operator<(T other) const;
	constexpr void negate();
	constexpr auto operator-() const;
	constexpr auto operator!=(rational other) const;
	explicit constexpr operator T() const;
```

Trieda reprezentujúca racionálne číslo.

### shift

```cpp
struct shift
{
	shift(size_t count);
};

constexpr view auto operator<<(view auto&& v, shift s);
constexpr view auto operator<<(shift s, view auto&& v);
constexpr view auto operator<(view auto&& v, shift s);
constexpr view auto operator<(shift s, view auto&& v);
constexpr view auto operator>(view auto&& v, shift s);
constexpr view auto operator>(shift s, view auto&& v);
```

Spolu s niektorým z operatorov vracia posunutý `view`.

`v << shift(n)` posunie begin a end o `n` späť\
`v >> shift(n)` posunie begin a end o `n` dopredu\
`v < shift(n)` posunie end o `n` späť\
`v > shift(n)` posunie end o `n` dopredu\
`shift(n) < v` posunie begin o `n` späť\
`shift(n) > v` posunie begin o `n` dopredu
