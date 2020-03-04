# PPmatrix

Knižnica pre prácu s maticami.

## Operácie na maticiach

* sčítanie
* násobenie matice maticou a skalárom
* transpozícia
* elementárne riadkové operácie
* úprava na REF a RREF
* rank
* determinant
* riešenie sústavy linárnych rovníc

## Technické detaily

Knižnica implementuje všetky maticové operácie ako nečlenské šablónové funkcie.

Definuje "myšlienkový koncept" `MatrixView`, ktorý tieto funkcie používajú.

Typ `T` spĺňa koncept `MatrixView`, ak sa na jeho objekt `M` dajú zavolať funkcie `begin(M)`, `end(M)` a `width(M).`

Nedefinuje `MatrixView` ako concept z C++20, hoci by to bolo možné.

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

## Dokumentácia

[link](documentation/main.md)
