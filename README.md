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

Definuje "myšlienkové koncepty" `View` a `MatrixView`, ktoré tieto funkcie používajú.\
Nedefinuje `View` a `MatrixView` ako concept z C++20, hoci by to bolo možné.\
Typ `T` spĺňa koncept `View`, ak sa na jeho objekt `V` dajú zavolať funkcie `begin(V)` a `end(V)`.\
Typ `T` spĺňa koncept `MatrixView`, ak spĺňa `View` a na jeho objekt `M` sa dá zavolať funkcia `width(M)`.

Implementuje generické algoritmy pracujúce s konceptom `View` (napr. [PPmatrix::copy](reference#copy)).



## Príklad

```
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

## Licencia

[MIT License](LICENSE)
