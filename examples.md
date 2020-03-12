# Príklady

### Načítanie matice zo vstupu po riadkoch

```cpp
int width, height;

std::cin >> width >> height;

PPmatrix::dynamic_matrix<int> matrix(height, width);

for (auto&& element : matrix)
	std::cin >> element;

PPmatrix::pretty_print(std::cout, matrix);
```

vstup:

```
3
2
1 2 5
4 7 8
```

výstup:

```
|1 2 5|
|4 7 8|
```

### Načítanie matice zo vstupu po stĺpcoch pomocou transpozície

```cpp
int width, height;

std::cin >> width >> height;

// flip dimensions
PPmatrix::dynamic_matrix<int> matrix(width, height);

for (auto&& element : matrix)
	std::cin >> element;

PPmatrix::transpose(matrix);

// flip back
matrix.flip_dimension();

PPmatrix::pretty_print(std::cout, matrix);
```

vstup:

```
3
2
1 1
2 2
3 3
```

výstup:

```
|1 2 3|
|1 2 3|
```

### Načítanie matice zo vstupu po stĺpcoch pomocou PPmatrix::column

```cpp
int width, height;

std::cin >> width >> height;

PPmatrix::dynamic_matrix<int> matrix(height, width);

for (auto&& column : PPmatrix::columns_sentinel(matrix))
	for (auto&& element : column)
		std::cin >> element;

PPmatrix::pretty_print(std::cout, matrix);
```

vstup:

```
3
2
1 1
2 2
3 3
```

výstup:

```
|1 2 3|
|1 2 3|
```

### Vypisovanie pomocou algoritmu

```cpp
// copy prijíma dva view a kopíruje, kým nie je u nejakého na konci,
// preto treba z std::ostream_iterator vytvoriť nekonečný view
PPmatrix::copy(
	{ 5, 4, 7, 3, 6 , 8, 4 },
	std::ostream_iterator<int>(std::cout, " wow ") ^ PPmatrix::unbounded);
```

výstup:

```
5 wow 4 wow 7 wow 3 wow 6 wow 8 wow 4 wow 
```

### Maximálny prvok v absolútnej hodnote

```cpp
std::vector<int> numbers;

{
	int x;
	while (std::cin)
	{
		std::cin >> x;
		numbers.push_back(x);
	}
}

PPmatrix::transform abs = [](int x) { return std::abs(x); };

// max_element vráti iterator rovnakého typu ako ten,
// ktorý mu bol predaný
// ak chceme vypísať pôvodnú hodnotu musíme zavolat .base(),
// inak by vypísalo 6, nie -6
std::cout << *PPmatrix::max_element(numbers | abs).base() << '\n';
```

vstup:

```
0 1 2 3 -6 5 x
```

výstup:

```
-6
```
