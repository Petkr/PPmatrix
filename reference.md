# PPmatrix

Referencia zápočtového programu PPmatrix.

# Obsah

* [Koncepty](#Koncepty)
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
  * [unbounded_sentinel](#unbounded_sentinel)
  * [rational](#rational)

## Koncepty

### view

```
template <typename View>
concept view = requires (View v)
{
	begin(v);
	end(v);
};
```

### matrix_view
```
template <typename MatrixView>
concept matrix_view =
	view<MatrixView> &&
	requires (const MatrixView mv)
	{
		width(mv);
	};
```

## Maticové operácie

### multiply

(1)

```
template <
	std::size_t flags = flag::none,
	typename MatrixViewA,
	typename MatrixViewB,
	typename MatrixViewResult>
constexpr void multiply(const MatrixViewA& A, const MatrixViewB& B, MatrixViewResult& result);
```

Násobí maticu `A` maticou `B`. Výsledok uloží do matice `result`.

`flags::size` zapne kontrolu správnych rozmerov matíc.

(2)

```
template <
	typename MatrixView,
	typename T>
constexpr void multiply(MatrixView& matrix, const T& scalar);
```

Násobí maticu `matrix` skalárom `scalar`.

### add

```
template <
	std::size_t flags = flag::none,
	typename MatrixViewA,
	typename MatrixViewB>
constexpr auto& add(MatrixViewA& A, const MatrixViewB& B);
```

Pripočíta k matici `A` maticu `B`. Referenciou vráti `A` po pričítaní.

`flags::size` zapne kontrolu správnych rozmerov matíc.

### transpose

(1)

```
template <
	bool const_square = false,
	typename MatrixView>
constexpr auto transpose(MatrixView& matrix, bool square);
```

Vráti transponovanú maticu `matrix`.

Ak je `const_square` alebo `square` `true`,
transponuje, akoby bola `matrix` štvorcová matica.\
Inak použije
[všeobecný algoritmus](https://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose).

`transpose<true>(matrix, b)` ignoruje `b`.

(2)

```
template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix);
```

Vráti transponovanú maticu `matrix`.

`transpose<true>(matrix)` volá `transpose<true>(matrix, _)`.\
`transpose<false>(matrix)` volá `transpose<true>(matrix, is_square(matrix))`.

### determinant

(1)

```
template <
	std::size_t flags = flag::none,
	typename MatrixView>
constexpr auto determinant(MatrixView& matrix, bool triangular);
```

Vráti determinant matice `matrix`.

`flags::square` zapne kontrolu, či je `matrix` štvorcová matica.

Ak je nastavené `flags::triangular` alebo `triangular` `true`,
spočíta determinant, akoby bola `matrix` trojuholníková matica.\
Inak použije všeobecný algoritmus.

Ak má `flags` nastavené `flags::triangular`, `determinant<flags>(matrix, b)` ignoruje `b`.

(2)

```
template <
	flag::bitmask flags = flag::none,
	typename MatrixView>
constexpr auto determinant(MatrixView& matrix);
```

Vráti determinant matice `matrix`.

`flags::square` ako pri (1).

Ak je nastavené `flags::triangular`, volá `determinant<flags>(matrix, _)`.\
Inak volá `determinant<flags>(matrix, is_triangular(matrix))`.

### solve_linear_equations

```
template <
	flag::bitmask flags = flag::none,
	typename MatrixView,
	typename MatrixViewVector>
constexpr auto solve_linear_equations(MatrixView& M, MatrixViewVector& v);
```

Pre rovnicu `Mb = v`:

Ak neexistuje `b`, vráti -1.\
Ak existuje, vráti počet dimenzií riešenia.

Ak existuje práve jedno riešenie, uloží ho do `v`, inak nechá prvky `v` v neznámom stave.

`flag::height` zapne kontrolu rozmerov matíc.

### REF

```
template <
	bool reduced = false,
	bool calculate_determinant = false,
	bool calculate_rank = false,
	typename MatrixView>
constexpr auto REF(MatrixView& matrix);
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

```
template <
	typename T,
	std::size_t size>
class static_matrix
{
	constexpr explicit static_matrix(std::size_t width);

	constexpr auto begin();
	constexpr auto begin() const;
	constexpr auto end();
	constexpr auto end() const;
	constexpr auto width() const;
};
```

Container so statickou veľkosťou spĺňajúci `matrix_view`.

### dynamic_matrix

```
template <typename T>
class dynamic_matrix
{
	dynamic_matrix(std::size_t height, std::size_t width);

	auto begin();
	auto begin() const;
	auto end();
	auto end() const;
	auto width() const;
	void resize(std::size_t new_height);
	void resize(std::size_t new_width, resize_columns_tag_t);
};
```

Container s dynamickou veľkosťou spĺňajúci `matrix_view`.

## Views

### simple_view

```
	template <
		typename Iterator,
		typename Sentinel>
	class simple_view
	{
	public:
		template <view View>
		constexpr simple_view(View&& v);
		constexpr simple_view(Iterator begin, Sentinel end);
		constexpr auto begin() const;
		constexpr auto end() const;
	};
```

Ukladá dvojicu iteratorov begin a end. Spĺňa `view`.

### simple_matrix_view

```
	template <
		typename Iterator,
		typename Sentinel>
	class simple_matrix_view
	{
	public:
		template <view View>
		constexpr simple_matrix_view(View&& v, std::size_t width);
		template <matrix_view MatrixView>
		constexpr simple_matrix_view(MatrixView&& v);
		constexpr simple_matrix_view(Iterator begin, Sentinel end, std::size_t width);
		constexpr auto begin() const;
		constexpr auto end() const;
		constexpr auto width() const;
		constexpr auto set_width(std::size_t width);
	};
```

Ukladá dvojicu iteratorov begin a end a šírku matice. Spĺňa `matrix_view`.

### augmented_matrix_view

```
template <
	typename LeftMatrixView,
	typename RightMatrixView>
class augmented_matrix_view
{
	template <
		matrix_view LeftMatrixViewAny,
		matrix_view RightMatrixViewAny>
	augmented_matrix_view(LeftMatrixViewAny&& A, RightMatrixViewAny&& B);

	template <
		matrix_view LeftMatrixViewAny,
		matrix_view RightMatrixViewAny>
	augmented_matrix_view(LeftMatrixViewAny&& left, RightMatrixViewAny&& right, dont_check_heights_tag_t);

	auto begin() const;
	auto end() const;
	auto width() const;
	auto size() const;
};

template <
	typename LeftMatrixViewAny,
	typename RightMatrixViewAny>
augmented_matrix_view(LeftMatrixViewAny&& A, RightMatrixViewAny&& B)
->	augmented_matrix_view<
	decltype(simple_matrix_view(A)),
	decltype(simple_matrix_view(B))>;
```

Reprezentuje
[rozšírenú maticu sústavy](https://en.wikipedia.org/wiki/Augmented_matrix)
(A | B).

Spĺňa `matrix_view`.

## Generické algoritmy

Algoritmy, ktoré akceptujú `view` na konštantné prvky
majú navyše overloady pre `std::initializer_list`.
Keďže výraz `{1, 2, 3}` nemá v C++ typ,
kompilátor nedokáže vydedukovať template parameter.

### accumulate

(1)

```
template <
	view View,
	typename BinaryFunction,
	typename T = view_base_t<View>>
constexpr auto accumulate(const View& v, BinaryFunction f, T init = {});
```

Vráti ľavý fold `` cez funkciu `f` s počiatočnou hodnotou `init`.

Napríklad pre `v` s prvkami `{ 1, 2, 5, 9 }`, funkciu `f` a `init` rovný `0`
vráti `f(f(f(f(init, 1), 2), 5), 9)`.

(2)

```
template <
	typename T,
	typename BinaryFunction,
	typename U = view_base_t<std::initializer_list<T>>>
constexpr auto accumulate(const std::initializer_list<T>& l, BinaryFunction f, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### copy

(1)

```
template <
	view ViewFrom,
	view ViewTo>
constexpr void copy(const ViewFrom& from, ViewTo&& to);
```

Nakopíruje `from` do `to` po prvkoch pomocou `operator=`.

Počet volaní `operator=` je rovný `std::min(PPmatrix::size(from), PPmatrix::size(to))`.

(2)

```
template <
	typename T,
	view ViewTo>
constexpr void copy(const std::initializer_list<T>& from, ViewTo&& to);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### equal

(1)

```
template <
	view View1,
	view View2>
constexpr auto equal(View1&& view1, View2&& view2);
```

Určí, či sú `view1` a `view2` zhodné po prvkoch podľa `operator==`.

Z oboch `view` porovnáva iba prvých `std::min(PPmatrix::size(view1), PPmatrix::size(view2))` prvkov.

(2)

```
template <
	typename T,
	view View2>
constexpr auto equal(const std::initializer_list<T>& l, View2&& view2);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```
template <
	view View1,
	typename T>
constexpr auto equal(View1&& view1, const std::initializer_list<T>& l);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```
template <
	typename T,
	typename U>
constexpr auto equal(const std::initializer_list<T>& l, const std::initializer_list<U>& m);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### fill

```
template <
	view View,
	typename T>
constexpr void fill(View&& view, const T& value);
```

### find

(1)

```
template <
	view View,
	typename UnaryPredicate>
constexpr auto find(View&& v, UnaryPredicate p);
```

Ak existuje element `x` vo `v`, pre ktorý `p(x)` vráti `true`, vráti iterator na prvý taký.\
Inak vráti iterator rovný `PPmatrix::end(view)`.

(2)

```
template <
	typename T,
	typename UnaryPredicate>
constexpr auto find(const std::initializer_list<T>& l, UnaryPredicate p);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### inner_product

(1)

```
template <
	view View1,
	view View2,
	typename T = detail::accumulator_t<View1, View2>>
constexpr auto inner_product(const View1& view1, const View2& view2, T init = {});
```

Vráti súčet `init` a súčinov prvkov `view1` a `view2`.

(2)

```
template <
	typename T,
	view View2,
	typename U = detail::accumulator_t<std::initializer_list<T>, View2>>
constexpr auto inner_product(const std::initializer_list<T>& l, const View2& view2, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```
template <
	view View1,
	typename T,
	typename U = detail::accumulator_t<View1, std::initializer_list<T>>>
constexpr auto inner_product(const View1& view1, const std::initializer_list<T>& l, U init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```
template <
	typename T,
	typename U,
	typename V = detail::accumulator_t<std::initializer_list<T>, std::initializer_list<U>>>
constexpr auto inner_product(const std::initializer_list<T>& l, const std::initializer_list<U>& m, V init = {});
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### max_element

(1)

```
template <view View>
constexpr auto max_element(View&& v);
```

Vráti iterator na najväčší prvok vo `v`.

(2)

```
template <typename T>
constexpr auto max_element(const std::initializer_list<T>& l);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### swap_ranges

```
template <
	view View1,
	view View2>
constexpr void swap_ranges(View1&& view1, View2&& view2);
```

Prehodí prvky `view1` a `view2` použitím `std::swap`.

### zip

(1)

```
template <
	typename Function,
	view ViewV,
	view ViewW>
constexpr void zip(ViewV&& v, ViewW&& w, Function f);
```

Zavolá `f` na jednotlivé prvky `v` a `w`. Teda pre prvky `a` a `b` zavolá `f(a, b)`.

Počet volaní `f` je rovný `std::min(PPmatrix::size(v), PPmatrix::size(w))`.

(2)

```
template <
	typename Function,
	typename T,
	view ViewW>
constexpr void zip(const std::initializer_list<T>& l, ViewW&& w, Function f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(3)

```
template <
	typename Function,
	view ViewV,
	typename T>
constexpr void zip(ViewV&& v, const std::initializer_list<T>& l, Function f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

(4)

```
template <
	typename Function,
	typename T,
	typename U>
constexpr void zip(const std::initializer_list<T>& l, const std::initializer_list<U>& m, Function f);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

## Iterator adaptory

### skip_iterator

```
template <typename BaseIterator>
class skip_iterator
{
public:
	constexpr skip_iterator(BaseIterator base_iterator, std::size_t skip_length);

	constexpr decltype(auto) operator*();
	constexpr auto& operator+=(std::size_t offset);
	constexpr auto& operator-=(std::size_t offset);
	constexpr auto operator!=(BaseIterator other);
	constexpr auto operator!=(skip_iterator other);
	constexpr auto& base();
};

struct skip
{
	skip(std::size_t skip_length);
};
```

Pre `skip_iterator si(i, sl)`:\
`si += n` volá `i += n * sl`\
`*si` vracia `*i`.

### transform_iterator

```
template <
	typename BaseIterator,
	typename Transform>
class transform_iterator
{
public:
	constexpr transform_iterator(BaseIterator base_iterator, Transform t);

	constexpr decltype(auto) operator*();
	constexpr auto& operator+=(std::size_t offset);
	constexpr auto& operator-=(std::size_t offset);
	template <typename OtherIterator>
	constexpr auto operator!=(OtherIterator other_iterator) const;
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

```
template <typename T>
class static_iterator
{
public:
	constexpr static_iterator(T&& value);
	constexpr static_iterator(const T& value);

	constexpr auto& operator*();
	constexpr auto& operator+=(std::size_t);
	constexpr auto& operator-=(std::size_t);
};
```

Iterator, ktorý simuluje nekonečný `view` s rovnakými prvkami.\
Jeho `operator+=` nerobí nič a `operator*` vracia obalený objekt.

##### static_view

```
template <typename T>
constexpr auto static_view(T&& value)
```

Wrapper, ktorý vracia object spĺňajúci `view`.

### wrap_iterator

```
template <typename T>
class wrap_iterator
{
	T value;
public:
	constexpr wrap_iterator(const T& value);
	constexpr wrap_iterator(T&& value);

	constexpr auto& operator*();
	constexpr const auto& operator*() const;
	constexpr auto& operator+=(std::size_t offset);
	constexpr auto operator!=(const T& other) const;
	template <typename U>
	constexpr auto operator!=(const wrap_iterator<U>& other) const;
};
```

Pre `wrap_iterator wi(x)`:\
`wi += n` volá `x += n`\
`*wi` vracia `x`.


##### wrap_view

(1)

```
template <typename T, typename U>
constexpr auto wrap_view(T&& begin, U&& end);
```

Vracia [`view`](#view), ktorý má begin a end typu [`wrap_iterator`](#wrap_iterator).

(2)

```
template <typename T>
constexpr auto wrap_view(T&& begin);
```

Vracia [`unbounded`](#unbounded) [`view`](#view), ktorý má begin typu
[`wrap_iterator`](#wrap_iterator).

### take_iterator

```
template <typename BaseIterator>
class take_iterator
{
public:
	constexpr take_iterator(BaseIterator base_iterator, std::size_t count);

	constexpr decltype(auto) operator*();
	constexpr auto& operator+=(std::size_t offset);
	template <typename OtherIterator>
	constexpr auto operator!=(OtherIterator i) const;
};
```

Iterator, ktorý sám simuluje `view`, ktorý si namiesto end pamätá svoju veľkosť.

##### take

```
struct take
{
	constexpr take(std::size_t count);
};
```

Pozri [použitie]().

##### take_view

```
template <typename Iterator>
constexpr auto take_view(Iterator i, std::size_t n);
```

Wrapper, ktorý vracia object spĺňajúci `view`.

## Ostatné

### unbounded

```
struct unbounded_t {};
constexpr unbounded_t unbounded{};
```

Pre každý iterator `i` vráti `i != unbounded` `false`.

[`view`](#view), ktorý má `unbounded` ako svoj end je nekonečný.

### rational

```
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

```
struct shift
{
	shift(std::size_t count);
};

template <view View>
constexpr auto operator<<(View&& v, shift s);

template <view View>
constexpr auto operator<<(shift s, View&& v);

template <view View>
constexpr auto operator<(View&& v, shift s);

template <view View>
constexpr auto operator<(shift s, View&& v);

template <view View>
constexpr auto operator>(View&& v, shift s);

template <view View>
constexpr auto operator>(shift s, View&& v);
```

Spolu s niektorým z operatorov vracia posunutý `view`.

`v << shift(n)` posunie begin a end o `n` späť\
`v >> shift(n)` posunie begin a end o `n` dopredu\
`v < shift(n)` posunie end o `n` späť\
`v > shift(n)` posunie end o `n` dopredu\
`shift(n) < v` posunie begin o `n` späť\
`shift(n) > v` posunie begin o `n` dopredu
