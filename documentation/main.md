# PPmatrix

Dokumentácia zápočtového programu PPmatrix.

## Obsah

* [multiply](#multiply)
* [add](#add)


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