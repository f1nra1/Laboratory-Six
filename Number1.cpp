#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include "main.h"

void task1();
void task2();
void task3();

enum class Number1{
    Matrix1 = 1,
    Matrix2,
    Matrix3
};

void Matrix(){
    while (true) {
        int num;
        cout << "Выберите шифр:" << endl
             << "1. Матрица MxN" << endl
             << "2. Квадратная матрица" << endl
             << "3. Морской бой" << endl
             << "4. Выход" << endl
             << "Ваш выбор: ";

        try {
            cin >> num;
            if (num == 4) {
                system("clear");
                break;
            }

            Number1 ex = static_cast<Number1>(num);
            switch (ex) {
                case Number1::Matrix1:
                    system("clear");
                    task1();
                    break;
                case Number1::Matrix2:
                    system("clear");
                    task2();
                    break;
                case Number1::Matrix3:
                    system("clear");
                    task3();
                    break;
                default:
                    cout << "Неверный выбор!" << endl;
                    break;
            }


        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    }
}

// Задача 1: Поиск строки с наибольшим количеством отрицательных чисел
void task1() {
    cout << "=== ЗАДАЧА 1 ===" << endl;

    // Инициализация генератора случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(-10, 10);
    uniform_int_distribution<> dis2(8, 15);
    uniform_int_distribution<> dis3(6, 15);

    // Размеры матрицы
    const int M = dis2(gen); // M > 7
    const int N = dis3(gen); // N > 5

    // Создание и заполнение матрицы
    vector<vector<int>> matrix(M, vector<int>(N));

    cout << "Исходная матрица " << M << "x" << N << ":" << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis1(gen);
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Поиск строки с наибольшим количеством отрицательных чисел
    int maxNegativeCount = 0;
    int rowWithMaxNegatives = 0;

    for (int i = 0; i < M; i++) {
        int negativeCount = 0;
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] < 0) {
                negativeCount++;
            }
        }

        if (negativeCount > maxNegativeCount) {
            maxNegativeCount = negativeCount;
            rowWithMaxNegatives = i;
        }

        cout << "Строка " << i << ": " << negativeCount << " отрицательных чисел" << endl;
    }

    cout << "\nСтрока с наибольшим количеством отрицательных чисел: "
         << rowWithMaxNegatives << " (содержит " << maxNegativeCount
         << " отрицательных чисел)" << endl;

    // Создание нового массива из найденной строки
    vector<int> newArray(matrix[rowWithMaxNegatives]);

    cout << "Новый массив, инициализированный найденной строкой:" << endl;
    for (int val : newArray) {
        cout << setw(4) << val << " ";
    }
    cout << endl << endl;
}

// Задача 2: Работа с диагоналями квадратной матрицы
void task2() {
    cout << "=== ЗАДАЧА 2 ===" << endl;

    // Инициализация генератора случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(10, 100);
    uniform_int_distribution<> dis2(6, 9);

    // Размер квадратной матрицы
    const int M = dis2(gen); // M > 5

    // Создание и заполнение матрицы
    vector<vector<int>> matrix(M, vector<int>(M));

    cout << "Исходная квадратная матрица " << M << "x" << M << ":" << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = dis1(gen);
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // Извлечение главной диагонали
    vector<int> mainDiagonal;
    for (int i = 0; i < M; i++) {
        mainDiagonal.push_back(matrix[i][i]);
    }

    // Извлечение побочной диагонали
    vector<int> secondaryDiagonal;
    for (int i = 0; i < M; i++) {
        secondaryDiagonal.push_back(matrix[i][M - 1 - i]);
    }

    cout << "\nГлавная диагональ до сортировки: ";
    for (int val : mainDiagonal) {
        cout << val << " ";
    }
    cout << endl;

    cout << "Побочная диагональ до сортировки: ";
    for (int val : secondaryDiagonal) {
        cout << val << " ";
    }
    cout << endl;

    // Функция для извлечения и сортировки цифр из числа
    auto extractAndSortDigits = [](int num) {
        vector<int> digits;
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
        sort(digits.begin(), digits.end(), greater<int>());

        int result = 0;
        for (int digit : digits) {
            result = result * 10 + digit;
        }
        return result;
    };

    // Сортировка цифр в элементах диагоналей
    for (int& val : mainDiagonal) {
        val = extractAndSortDigits(val);
    }

    for (int& val : secondaryDiagonal) {
        val = extractAndSortDigits(val);
    }

    cout << "\nГлавная диагональ после сортировки цифр: ";
    int sumMain = 0;
    for (int val : mainDiagonal) {
        cout << val << " ";
        sumMain += val;
    }
    cout << "\nСумма элементов главной диагонали: " << sumMain << endl;

    cout << "\nПобочная диагональ после сортировки цифр: ";
    int sumSecondary = 0;
    for (int val : secondaryDiagonal) {
        cout << val << " ";
        sumSecondary += val;
    }
    cout << "\nСумма элементов побочной диагонали: " << sumSecondary << endl;

    if (sumMain > sumSecondary) {
        cout << "\nНаибольшая сумма у главной диагонали (" << sumMain << ")" << endl;
    } else if (sumSecondary > sumMain) {
        cout << "\nНаибольшая сумма у побочной диагонали (" << sumSecondary << ")" << endl;
    } else {
        cout << "\nСуммы диагоналей равны (" << sumMain << ")" << endl;
    }

    // Обновление матрицы с отсортированными диагоналями
    for (int i = 0; i < M; i++) {
        matrix[i][i] = mainDiagonal[i];
        matrix[i][M - 1 - i] = secondaryDiagonal[i];
    }

    cout << "\nМатрица после сортировки цифр в диагоналях:" << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Задача 3: Генерация случайной доски и подсчет кораблей в игре "Морской бой"
void task3() {
    cout << "=== ЗАДАЧА 3 ===" << endl;

    random_device rd;
    mt19937 gen(rd());

    const int m = 10;
    const int n = 10;
    vector<vector<char>> board(m, vector<char>(n, '.'));

    auto canPlaceShip = [&](int x, int y, int size, bool horizontal) {
        if (horizontal && y + size > n) return false;
        if (!horizontal && x + size > m) return false;

        for (int i = max(0, x - 1); i <= min(m - 1, x + (horizontal ? 1 : size)); i++) {
            for (int j = max(0, y - 1); j <= min(n - 1, y + (horizontal ? size : 1)); j++) {
                if (board[i][j] == 'X') return false;
            }
        }
        return true;
    };

    auto placeShip = [&](int x, int y, int size, bool horizontal) {
        if (horizontal) {
            for (int j = y; j < y + size; j++) board[x][j] = 'X';
        } else {
            for (int i = x; i < x + size; i++) board[i][y] = 'X';
        }
    };

    vector<int> ships;
    uniform_int_distribution<> ship_count_dis(5, 10);
    int totalShips = ship_count_dis(gen);

    uniform_int_distribution<> ship_size_dis(1, 4);
    int maxShipCells = (m * n) / 5;
    int currentShipCells = 0;

    for (int i = 0; i < totalShips; i++) {
        int size = ship_size_dis(gen);
        if (currentShipCells + size <= maxShipCells) {
            ships.push_back(size);
            currentShipCells += size;
        }
    }

    sort(ships.begin(), ships.end(), greater<int>());

    uniform_int_distribution<> dis_x(0, m - 1);
    uniform_int_distribution<> dis_y(0, n - 1);
    uniform_int_distribution<> dis_dir(0, 1);

    for (int size : ships) {
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 100) {
            int x = dis_x(gen);
            int y = dis_y(gen);
            bool horizontal = dis_dir(gen);

            if (canPlaceShip(x, y, size, horizontal)) {
                placeShip(x, y, size, horizontal);
                placed = true;
            }
            attempts++;
        }
    }

    vector<vector<bool>> visited(m, vector<bool>(n, false));

    function<void(int, int)> markShip = [&](int x, int y) {
        if (x < 0 || x >= m || y < 0 || y >= n || visited[x][y] || board[x][y] != 'X')
            return;

        visited[x][y] = true;
        markShip(x + 1, y);
        markShip(x - 1, y);
        markShip(x, y + 1);
        markShip(x, y - 1);
    };

    int shipCount = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'X' && !visited[i][j]) {
                shipCount++;
                markShip(i, j);
            }
        }
    }

    cout << "  ";
    for (int j = 0; j < n; j++) cout << j << " ";
    cout << "\n  ";
    for (int j = 0; j < n; j++) cout << "--";
    cout << "\n";

    for (int i = 0; i < m; i++) {
        cout << i << "|";
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\nКоличество кораблей на доске: " << shipCount << "\n";
}