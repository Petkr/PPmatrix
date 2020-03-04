# PPmatrix

Referencia zápočtového programu PPmatrix.

# Obsah

* [Maticové operácie]
* [multiply]
* [add]
* [transpose]
* [determinant]
* [solve_linear_equations]
* [REF]
* [copy]
* [augmented_matrix_view]

## Maticové operácie

### multiply

(1)

```
template <std::size_t flags = flag::none, typename MatrixViewA, typename MatrixViewB, typename MatrixViewResult>
constexpr void multiply(const MatrixViewA& A, const MatrixViewB& B, MatrixViewResult& result);
```

Násobí maticu `A` maticou `B`. Výsledok uloží do matice `result`.

`flags::size` zapne kontrolu správnych rozmerov matíc.

(2)

```
template <typename MatrixView, typename T>
constexpr void multiply(MatrixView& matrix, const T& scalar);
```

Násobí maticu `matrix` skalárom `scalar`.

### add

```
template <std::size_t flags = flag::none, typename MatrixViewA, typename MatrixViewB>
constexpr auto& add(MatrixViewA& A, const MatrixViewB& B);
```

Pripočíta k matici `A` maticu `B`. Referenciou vráti `A` po pričítaní.

`flags::size` zapne kontrolu správnych rozmerov matíc.

### transpose

(1)

```
template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix, bool square);
```

Vráti transponovanú maticu `matrix`.

Ak je `const_square` alebo `square` `true`, transponuje, akoby bola `matrix` štvorcová matica.\
Inak použije [všeobecný algoritmus](https://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose).

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
template <std::size_t flags = flag::none, typename MatrixView>
constexpr auto determinant(MatrixView& matrix, bool triangular);
```

Vráti determinant matice `matrix`.

`flags::square` zapne kontrolu, či je `matrix` štvorcová matica.

Ak je nastavené `flags::triangular` alebo `triangular` `true`, spočíta determinant, akoby bola `matrix` trojuholníková matica.\
Inak použije všeobecný algoritmus.

Ak má `flags` nastavené `flags::triangular`, `determinant<flags>(matrix, b)` ignoruje `b`.

(2)

```
template <flag::bitmask flags = flag::none, typename MatrixView>
constexpr auto determinant(MatrixView& matrix);
```

Vráti determinant matice `matrix`.

`flags::square` ako pri (1).

Ak je nastavené `flags::triangular`, volá `determinant<flags>(matrix, _)`.\
Inak volá `determinant<flags>(matrix, is_triangular(matrix))`.

### solve_linear_equations

```
template <flag::bitmask flags = flag::none, typename MatrixView, typename MatrixViewVector>
constexpr std::make_signed_t<std::size_t> solve_linear_equations(MatrixView& M, MatrixViewVector& v);
```

Do `v` uloží `b`, ktoré je riešením rovnice `Mb = v`.

`flag::height` zapne kontrolu rozmerov matíc.

### REF

```
template <bool reduced = false, bool calculate_determinant = false, bool calculate_rank = false, typename MatrixView>
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

Algoritmy, ktoré akceptujú `const&` argument majú navyše overloady pre `std::initializer_list`,
keďže výraz `{1, 2, 3}` nemá v C++ typ, teda kompilátor nedokáže vydedukovať template parameter.

### copy

(1)

```
template <typename ViewFrom, typename ViewTo>
constexpr void copy(const ViewFrom& from, ViewTo&& to);
```

Nakopíruje `from` do `to` po prvkoch pomocou `operator=`.

Počet volaní `operator=` je rovný `std::min(PPmatrix::size(from), PPmatrix::size(to))`

(2)

```
template <typename T, typename ViewTo>
constexpr void copy(const std::initializer_list<T>& from, ViewTo&& to);
```

Zhodné s (1). Pozri [vysvetlenie](#generické-algoritmy).

## augmented_matrix_view

```
template <typename LeftMatrixView, typename RightMatrixView>
class augmented_matrix_view
{
	template <typename LeftMatrixViewAny, typename RightMatrixViewAny>
	augmented_matrix_view(LeftMatrixViewAny&& A, RightMatrixViewAny&& B);

	auto begin() const;
	auto end() const;
	auto width() const;
	auto size() const;
};

template <typename LeftMatrixViewAny, typename RightMatrixViewAny>
augmented_matrix_view(LeftMatrixViewAny&& A, RightMatrixViewAny&& B)
->	augmented_matrix_view<
	decltype(simple_matrix_view(A)),
	decltype(simple_matrix_view(B))>;
```

Umožňuje vytvoriť `MatrixView` na [rozšírenú maticu sústavy](https://en.wikipedia.org/wiki/Augmented_matrix) (A | B).

Spĺňa `MatrixView`.
