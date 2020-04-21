# PPmatrix

Knižnica pre prácu s maticami inšpirovaná a čiastočne
využívajúca Ranges a Concepts z C++20.

Zápočtový program.

## Základné informácie

**Rozsah, v ktorom knižnica používa C++20,
implementuje dostatočne zatiaľ iba GCC 10.**

Obsahuje iba header súbory.

Implementuje všetky maticové operácie ako nonmember template funkcie.

Definuje koncepty [`view`](reference.md#view)
a [`matrix_view`](reference.md#matrix_view),
ktoré tieto funkcie používajú.

Knižnica predpokladá, že typy spĺňajúce
[`matrix_view`](reference.md#matrix_view)
majú prvky uložené *po riadkoch*.

Implementuje generické algoritmy pracujúce s konceptom [`view`](reference.md#view)
(napr. [`copy`](reference.md#copy)).

Poskytuje funkčné programovanie s lazy evaluation ako v Ranges:

```cpp
std::array<int, 6> a = { 1, 4, 6, 2, -5 };
PPmatrix::transform multiplier = [](auto&& x) { return x * 2; };
std::cout << *PPmatrix::max_element(a | multiplier);
// vypíše 12
```

## Implementované operácie na maticiach

* sčítanie
* násobenie matice maticou a skalárom
* transpozícia
* elementárne riadkové operácie
* úprava na REF a RREF
* rank
* determinant
* riešenie sústavy lineárnych rovníc

## Príklad

```cpp
PPmatrix::dynamic_matrix<int> A(2, 2);
PPmatrix::copy({ 1, 2, 1, 3 }, A);

PPmatrix::static_matrix<int, 2> b(1);
PPmatrix::copy({ 10, 13 }, b);

PPmatrix::pretty_print(std::cout, PPmatrix::augmented_matrix_view(A, b));

PPmatrix::solve_linear_equations(A, b);

PPmatrix::pretty_print(std::cout, b);
```

výstup:

```
| 1  2 10|
| 1  3 13|
|4|
|3|
```

[Viac ukážok kódu.](examples.md)

## Použitie

Knižnica pozostáva čisto z header súborov, takže stačí adresár PPmatrix
nakopírovať do nejakého include directory.

Súbor [PPmatrix.hpp](PPmatrix/PPmatrix.hpp)
include-uje všetky header súbory knižnice.

Pre testovanie sú súčasťou repository CMake súbory a [main.cpp](PPmatrix/main.cpp)
s ukážkovým kódom.

## Referencia

[reference](reference.md)

## Nedostatky

Knižnica na niektorých miestach ticho predpokladá, že iteratory sú
[contiguous iterator](https://en.cppreference.com/w/cpp/named_req/ContiguousIterator).
Na všetkých wrapper iteratoroch implementuje iba `operator+=`
a v [iterator.hpp](PPmatrix/iterator.hpp#L38) implementuje `operator++`
ako volanie `operator+=` s argumentom 1. Tiež používa `operator++`
na posunutie z posledného prvku riadka na prvý prvok nasledujúceho.

Iterator, ktorý používa
[`augmented_matrix_view`](reference.md#augmented_matrix_view),
je na iterator *obrovský*. Na 64-bit s internými pointer iteratormi má 56B.

[`augmented_matrix_view`](reference.md#augmented_matrix_view)
má hlúpo implementovaný `operator+=`, ktorý pre `x += n` volá n-krát `++x`.

## Licencia

[MIT License](LICENSE)
