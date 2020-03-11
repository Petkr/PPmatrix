# PPmatrix

Knižnica pre prácu s maticami inšpirovaná a čiastočne využívajúca Ranges a Concepts z C++20.

## Operácie na maticiach

* sčítanie
* násobenie matice maticou a skalárom
* transpozícia
* elementárne riadkové operácie
* úprava na REF a RREF
* rank
* determinant
* riešenie sústavy linárnych rovníc

## Ďalšie informácie

Knižnica implementuje všetky maticové operácie ako nečlenské šablónové funkcie.

Definuje koncepty
[`view`](reference.md#view)
a
[`matrix_view`](reference.md#matrix_view)
, ktoré tieto funkcie používajú.

Knižnica predpokladá, že typy spĺňajúce
[`matrix_view`](reference.md#matrix_view)
majú prvky uložené *po riadkoch*.

Implementuje generické algoritmy pracujúce s konceptom `view`
(napr. [`PPmatrix::copy`](reference.md#copy)).

Poskytuje funkčné programovanie s lazy evaluation ako v Ranges:

```cpp
std::array<int, 6> a = { 1, 4, 6, 2, -5 };
transform multiplier = [](auto&& x) { return x * 2; };
std::cout << *max_element(a | multiplier);
// vypíše 12
```

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

## Referencia

[reference](reference.md)

## Nedostatky

Knižnica na niektorých miestach ticho predpokladá, že iteratory sú random access.
Napríklad na všetkých wrapper iteratoroch implementuje iba `operator+=`
a v [operators.hpp](PPmatrix/operators.hpp)
implementuje `operator++` ako volanie `operator+=` s argumentom 1.

Iterator, ktorý používa
[`augmented_matrix_view`](reference.md#augmented_matrix_view),
je na iterator *obrovský*. Na 64-bit s internými iteratormi `T*` má 56B.

[`augmented_matrix_view`](reference.md#augmented_matrix_view)
má hlúpo implementovaný `operator+=`, ktorý pre `x += n` volá n-krát `++x`.

## Licencia

[MIT License](LICENSE)
