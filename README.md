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

## Dokumentácia

[link](documentation/index.md)
