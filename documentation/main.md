# PPmatrix

Dokumentácia zápočtového programu PPmatrix.

## Obsah

* [multiply](#multiply)
* [add](#add)
* [transpose](#transpose)

## multiply

(1)

```
template <std::size_t flags = flag::none, typename MatrixViewA, typename MatrixViewB, typename MatrixViewResult>
constexpr void multiply(const MatrixViewA& A, const MatrixViewB& B, MatrixViewResult& result)
```

Násobí maticu `A` maticou `B`. Výsledok uloží do matice `result`.

###### flags

`flags::size` - zapne kontrolu správnych rozmerov matíc

(2)

```
template <typename MatrixView, typename T>
constexpr void multiply(MatrixView& matrix, const T& scalar)
```

Násobí maticu `matrix` skalárom `scalar`.

## add

```
template <std::size_t flags = flag::none, typename MatrixViewA, typename MatrixViewB>
constexpr auto& add(MatrixViewA& A, const MatrixViewB& B)
```

Pripočíta k matici `A` maticu `B`. Referenciou vráti `A` po pričítaní.

###### flags

`flags::size` - zapne kontrolu správnych rozmerov matíc

## transpose

(1)

```
template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix, bool square)
```

Transponuje maticu `matrix`.

Ak je `const_square` alebo `square` `true`, transponuje, akoby bola `matrix` štvorcová. Inak použije všeobecný algoritmus.

`transpose<true>(matrix, b)` nekontroluje `b`.

(2)

```
template <bool const_square = false, typename MatrixView>
constexpr auto transpose(MatrixView& matrix)
```

Transponuje maticu `matrix`.

`transpose<true>(matrix)` volá `transpose<true>(matrix, _)`.

`transpose<false>(matrix)` volá `transpose<true>(matrix, is_square(matrix))`.
