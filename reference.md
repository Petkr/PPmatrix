# PPmatrix

Referencia zápočtového programu PPmatrix.

# Obsah

* [Maticové operácie](#maticové-operácie)
  * [multiply](#multiply)
  * [add](#add)
  * [transpose](#transpose)
  * [determinant](#determinant)
  * [solve_linear_equations](#solve_linear_equations)
  * [REF](#REF)
* [Generické algoritmy](#generické-algoritmy)
  * [copy](#copy)
* uvidzime
  * [augmented_matrix_view](#augmented_matrix_view)

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

## Generické algoritmy

Algoritmy, ktoré akceptujú `const T&` argument
majú navyše overloady pre `std::initializer_list`.
Keďže výraz `{1, 2, 3}` nemá v C++ typ,
kompilátor nedokáže vydedukovať template parameter.

### copy

(1)

```
template <
	typename ViewFrom,
	typename ViewTo>
constexpr void copy(const ViewFrom& from, ViewTo&& to);
```

Nakopíruje `from` do `to` po prvkoch pomocou `operator=`.

Počet volaní `operator=` je rovný `std::min(PPmatrix::size(from), PPmatrix::size(to))`.

(2)

```
template <
	typename T,
	typename ViewTo>
constexpr void copy(const std::initializer_list<T>& from, ViewTo&& to);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

### zip

(1)

```
template <
	typename Function,
	typename ViewV,
	typename ViewW>
constexpr void zip(ViewV&& v, ViewW&& w, Function f);
```

Zavolá `f` na jednotlivé prvky `v` a `w`. Teda pre prky `a` a `b` zavolá `f(a, b)`.

Počet volaní `f` je rovný `std::min(PPmatrix::size(v), PPmatrix::size(w))`.

(2)

```
template <
	typename Function,
	typename ViewV,
	typename ViewW,
	typename ViewZ>
constexpr void zip(ViewV&& v, ViewW&& w, ViewZ&& z, Function f);
```

Zavolá `f` na jednotlivé prvky `v`, `w` a `z`.
Teda pre prky `a`, `b` a `c` zavolá `f(a, b, c)`.

Počet volaní `f` je rovný
`std::min({ PPmatrix::size(v), PPmatrix::size(w), PPmatrix::size(z) })`.

### accumulate

(1)

```
template <
	typename View,
	typename BinaryFunction,
	typename T = view_base_t<View>>
constexpr auto accumulate(const View& view, BinaryFunction f, T init = {});
```

Vráti ľavý fold `view` cez funkciu `f` s počiatočnou hodnotou `init`.

Napríklad pre `View` s prvkami `{ 1, 2, 5, 9 }` a funkciu `f`
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

## uvidzime

### augmented_matrix_view

```
template <
	typename LeftMatrixView,
	typename RightMatrixView>
class augmented_matrix_view
{
	template <
		typename LeftMatrixViewAny,
		typename RightMatrixViewAny>
	augmented_matrix_view(LeftMatrixViewAny&& A, RightMatrixViewAny&& B);

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

Umožňuje vytvoriť `MatrixView` na
[rozšírenú maticu sústavy](https://en.wikipedia.org/wiki/Augmented_matrix)
(A | B).

Spĺňa `MatrixView`.

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
	void resize(std::size_t size, std::size_t width);
};
```