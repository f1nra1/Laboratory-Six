using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    enum Number1
    {
        Matrix1 = 1,
        Matrix2,
        Matrix3
    }

    static void Main(string[] args)
    {
        Matrix();
    }

    static void Matrix()
    {
        while (true)
        {
            Console.WriteLine("Выберите шифр:");
            Console.WriteLine("1. Матрица MxN");
            Console.WriteLine("2. Квадратная матрица");
            Console.WriteLine("3. Морской бой");
            Console.WriteLine("4. Выход");
            Console.Write("Ваш выбор: ");

            try
            {
                int num = int.Parse(Console.ReadLine());
                if (num == 4)
                {
                    Console.Clear();
                    break;
                }

                Number1 ex = (Number1)num;
                switch (ex)
                {
                    case Number1.Matrix1:
                        Console.Clear();
                        Task1();
                        break;
                    case Number1.Matrix2:
                        Console.Clear();
                        Task2();
                        break;
                    case Number1.Matrix3:
                        Console.Clear();
                        Task3();
                        break;
                    default:
                        Console.WriteLine("Неверный выбор!");
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Ошибка: {e.Message}");
            }
        }
    }

    static void Task1()
    {
        Console.WriteLine("=== ЗАДАЧА 1 ===");

        Random rnd = new Random();
        int M = rnd.Next(8, 16); // M > 7
        int N = rnd.Next(6, 16); // N > 5

        int[,] matrix = new int[M, N];

        Console.WriteLine($"Исходная матрица {M}x{N}:");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                matrix[i, j] = rnd.Next(-10, 11);
                Console.Write($"{matrix[i, j],4} ");
            }
            Console.WriteLine();
        }

        int maxNegativeCount = 0;
        int rowWithMaxNegatives = 0;

        for (int i = 0; i < M; i++)
        {
            int negativeCount = 0;
            for (int j = 0; j < N; j++)
            {
                if (matrix[i, j] < 0) negativeCount++;
            }

            if (negativeCount > maxNegativeCount)
            {
                maxNegativeCount = negativeCount;
                rowWithMaxNegatives = i;
            }

            Console.WriteLine($"Строка {i}: {negativeCount} отрицательных чисел");
        }

        Console.WriteLine($"\nСтрока с наибольшим количеством отрицательных чисел: {rowWithMaxNegatives} " +
                         $"(содержит {maxNegativeCount} отрицательных чисел)");

        int[] newArray = new int[N];
        for (int j = 0; j < N; j++)
        {
            newArray[j] = matrix[rowWithMaxNegatives, j];
        }

        Console.WriteLine("Новый массив, инициализированный найденной строкой:");
        foreach (int val in newArray)
        {
            Console.Write($"{val,4} ");
        }
        Console.WriteLine("\n");
    }

    static void Task2()
    {
        Console.WriteLine("=== ЗАДАЧА 2 ===");

        Random rnd = new Random();
        int M = rnd.Next(6, 10); // M > 5

        int[,] matrix = new int[M, M];

        Console.WriteLine($"Исходная квадратная матрица {M}x{M}:");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                matrix[i, j] = rnd.Next(10, 101);
                Console.Write($"{matrix[i, j],4} ");
            }
            Console.WriteLine();
        }

        int[] mainDiagonal = new int[M];
        int[] secondaryDiagonal = new int[M];

        for (int i = 0; i < M; i++)
        {
            mainDiagonal[i] = matrix[i, i];
            secondaryDiagonal[i] = matrix[i, M - 1 - i];
        }

        Console.WriteLine("\nГлавная диагональ до сортировки: " + string.Join(" ", mainDiagonal));
        Console.WriteLine("Побочная диагональ до сортировки: " + string.Join(" ", secondaryDiagonal));

        int SortDigits(int num)
        {
            return int.Parse(string.Join("", num.ToString()
                                                .ToCharArray()
                                                .OrderByDescending(c => c)));
        }

        for (int i = 0; i < M; i++)
        {
            mainDiagonal[i] = SortDigits(mainDiagonal[i]);
            secondaryDiagonal[i] = SortDigits(secondaryDiagonal[i]);
        }

        int sumMain = mainDiagonal.Sum();
        int sumSecondary = secondaryDiagonal.Sum();

        Console.WriteLine("\nГлавная диагональ после сортировки цифр: " + string.Join(" ", mainDiagonal));
        Console.WriteLine($"Сумма элементов главной диагонали: {sumMain}");

        Console.WriteLine("\nПобочная диагональ после сортировки цифр: " + string.Join(" ", secondaryDiagonal));
        Console.WriteLine($"Сумма элементов побочной диагонали: {sumSecondary}");

        if (sumMain > sumSecondary)
            Console.WriteLine($"\nНаибольшая сумма у главной диагонали ({sumMain})");
        else if (sumSecondary > sumMain)
            Console.WriteLine($"\nНаибольшая сумма у побочной диагонали ({sumSecondary})");
        else
            Console.WriteLine($"\nСуммы диагоналей равны ({sumMain})");

        for (int i = 0; i < M; i++)
        {
            matrix[i, i] = mainDiagonal[i];
            matrix[i, M - 1 - i] = secondaryDiagonal[i];
        }

        Console.WriteLine("\nМатрица после сортировки цифр в диагоналях:");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                Console.Write($"{matrix[i, j],4} ");
            }
            Console.WriteLine();
        }
        Console.WriteLine();
    }

    static void Task3()
    {
        Console.WriteLine("=== ЗАДАЧА 3 ===");

        Random rnd = new Random();
        const int m = 10;
        const int n = 10;
        char[,] board = new char[m, n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                board[i, j] = '.';

        bool CanPlaceShip(int x, int y, int size, bool horizontal)
        {
            if (horizontal && y + size > n) return false;
            if (!horizontal && x + size > m) return false;

            for (int i = Math.Max(0, x - 1); i <= Math.Min(m - 1, x + (horizontal ? 1 : size)); i++)
            {
                for (int j = Math.Max(0, y - 1); j <= Math.Min(n - 1, y + (horizontal ? size : 1)); j++)
                {
                    if (board[i, j] == 'X') return false;
                }
            }
            return true;
        }

        void PlaceShip(int x, int y, int size, bool horizontal)
        {
            if (horizontal)
                for (int j = y; j < y + size; j++) board[x, j] = 'X';
            else
                for (int i = x; i < x + size; i++) board[i, y] = 'X';
        }

        List<int> ships = new List<int>();
        int totalShips = rnd.Next(5, 11);
        int maxShipCells = (m * n) / 5;
        int currentShipCells = 0;

        for (int i = 0; i < totalShips; i++)
        {
            int size = rnd.Next(1, 5);
            if (currentShipCells + size <= maxShipCells)
            {
                ships.Add(size);
                currentShipCells += size;
            }
        }

        ships.Sort((a, b) => b.CompareTo(a));

        foreach (int size in ships)
        {
            bool placed = false;
            int attempts = 0;

            while (!placed && attempts < 100)
            {
                int x = rnd.Next(m);
                int y = rnd.Next(n);
                bool horizontal = rnd.Next(2) == 0;

                if (CanPlaceShip(x, y, size, horizontal))
                {
                    PlaceShip(x, y, size, horizontal);
                    placed = true;
                }
                attempts++;
            }
        }

        bool[,] visited = new bool[m, n];

        void MarkShip(int x, int y)
        {
            if (x < 0 || x >= m || y < 0 || y >= n || visited[x, y] || board[x, y] != 'X')
                return;

            visited[x, y] = true;
            MarkShip(x + 1, y);
            MarkShip(x - 1, y);
            MarkShip(x, y + 1);
            MarkShip(x, y - 1);
        }

        int shipCount = 0;
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i, j] == 'X' && !visited[i, j])
                {
                    shipCount++;
                    MarkShip(i, j);
                }
            }
        }

        Console.Write("  ");
        for (int j = 0; j < n; j++) Console.Write($"{j} ");
        Console.Write("\n  ");
        for (int j = 0; j < n; j++) Console.Write("--");
        Console.WriteLine();

        for (int i = 0; i < m; i++)
        {
            Console.Write($"{i}|");
            for (int j = 0; j < n; j++)
            {
                Console.Write($"{board[i, j]} ");
            }
            Console.WriteLine();
        }

        Console.WriteLine($"\nКоличество кораблей на доске: {shipCount}\n");
    }
}