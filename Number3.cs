using System;
using System.Linq;
using System.Collections.Generic;

class Program
{
    class IterationResult
    {
        public double[] X { get; set; }
        public bool Converged { get; set; }
        public int Iterations { get; set; }
        public double Error { get; set; }
    }

    static void PrintMatrix(double[,] A, double[] b = null)
    {
        int rows = A.GetLength(0);
        int cols = A.GetLength(1);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Console.Write($"{A[i,j],12:F6} ");
            }
            if (b != null) Console.Write($" | {b[i],12:F6}");
            Console.WriteLine();
        }
        Console.WriteLine();
    }

    static void PrintVector(double[] v, string title = "")
    {
        if (!string.IsNullOrEmpty(title)) Console.WriteLine(title);
        for (int i = 0; i < v.Length; i++)
        {
            Console.WriteLine($"x[{i}] = {v[i]:F6}");
        }
        Console.WriteLine();
    }

    static void ReorderForDiagonalDominance(double[,] A, double[] b)
    {
        int n = A.GetLength(0);
        for (int i = 0; i < n; i++)
        {
            int maxRow = i;
            double maxRatio = 0;

            for (int k = i; k < n; k++)
            {
                double sum = 0;
                for (int j = 0; j < n; j++)
                {
                    if (k != j) sum += Math.Abs(A[k,j]);
                }
                double ratio = Math.Abs(A[k,i]) / (sum + 1e-10);
                if (ratio > maxRatio)
                {
                    maxRatio = ratio;
                    maxRow = k;
                }
            }
            if (maxRow != i)
            {
                // Обмен строк в матрице A
                for (int j = 0; j < n; j++)
                {
                    var temp = A[i,j];
                    A[i,j] = A[maxRow,j];
                    A[maxRow,j] = temp;
                }
                // Обмен элементов в векторе b
                var tempB = b[i];
                b[i] = b[maxRow];
                b[maxRow] = tempB;
            }
        }
    }

    static IterationResult Seidel(double[,] A, double[] b, double epsilon = 1e-6, int maxIterations = 100)
    {
        int n = A.GetLength(0);
        const double omega = 0.5; // параметр релаксации
        double[] x = new double[n];
        double[] xPrev = new double[n];

        ReorderForDiagonalDominance(A, b);

        int iter = 0;
        double error;
        do
        {
            iter++;
            Array.Copy(x, xPrev, n);

            for (int i = 0; i < n; i++)
            {
                double sum = b[i];
                for (int j = 0; j < n; j++)
                {
                    if (j < i) sum -= A[i,j] * x[j];
                    else if (j > i) sum -= A[i,j] * xPrev[j];
                }
                x[i] = omega * (sum / A[i,i]) + (1 - omega) * xPrev[i];
            }

            error = 0;
            for (int i = 0; i < n; i++)
            {
                error = Math.Max(error, Math.Abs(x[i] - xPrev[i]));
            }

            if (iter % 10 == 0)
            {
                Console.WriteLine($"Итерация {iter}, погрешность: {error}");
            }

            if (iter >= maxIterations)
            {
                return new IterationResult { X = x, Converged = false, Iterations = iter, Error = error };
            }
        } while (error > epsilon);

        return new IterationResult { X = x, Converged = true, Iterations = iter, Error = error };
    }

    static double[] LUSolve(double[,] A, double[] b)
    {
        int n = A.GetLength(0);
        double[,] L = new double[n,n];
        double[,] U = new double[n,n];
        double[] y = new double[n];
        double[] x = new double[n];

        // LU разложение
        for (int i = 0; i < n; i++)
        {
            L[i,i] = 1;
            for (int j = i; j < n; j++)
            {
                U[i,j] = A[i,j];
                for (int k = 0; k < i; k++)
                    U[i,j] -= L[i,k] * U[k,j];
            }
            for (int j = i + 1; j < n; j++)
            {
                L[j,i] = A[j,i];
                for (int k = 0; k < i; k++)
                    L[j,i] -= L[j,k] * U[k,i];
                L[j,i] /= U[i,i];
            }
        }

        // Ly = b
        for (int i = 0; i < n; i++)
        {
            y[i] = b[i];
            for (int j = 0; j < i; j++)
                y[i] -= L[i,j] * y[j];
        }

        // Ux = y
        for (int i = n - 1; i >= 0; i--)
        {
            x[i] = y[i];
            for (int j = i + 1; j < n; j++)
                x[i] -= U[i,j] * x[j];
            x[i] /= U[i,i];
        }

        return x;
    }

    static void SolveEquations()
    {
        const int N = 4;
        double[,] A = {
            {-1.13, 0.04, 0.21, -18},
            {0.25, -1.23, 0.14, -0.09},
            {-0.21, 0.14, 0.8, -0.13},
            {0.15, -1.31, 0.06, 0.87}
        };
        double[] b = {-1.24, 0.87, 2.56, -1.13};

        Console.WriteLine("Решение СЛАУ методом Зейделя и LU-разложением\n");
        Console.WriteLine("Расширенная матрица [A|b]:");
        PrintMatrix(A, b);

        Console.WriteLine("=== Метод Зейделя ===");
        var result = Seidel(A, b);

        if (result.Converged)
        {
            Console.WriteLine($"Решение найдено за {result.Iterations} итераций");
            Console.WriteLine($"Финальная погрешность: {result.Error}");
            PrintVector(result.X, "Решение методом Зейделя:");
        }
        else
        {
            Console.WriteLine($"Метод не сошелся за {result.Iterations} итераций");
            Console.WriteLine($"Текущая погрешность: {result.Error}");
        }

        Console.WriteLine("\n=== LU-разложение ===");
        var lu_x = LUSolve(A, b);
        PrintVector(lu_x, "Решение методом LU-разложения:");

        Console.WriteLine("Невязка для метода Зейделя:");
        double[] r = new double[N];
        for (int i = 0; i < N; i++)
        {
            r[i] = -b[i];
            for (int j = 0; j < N; j++)
            {
                r[i] += A[i,j] * result.X[j];
            }
            Console.WriteLine($"r[{i}] = {r[i]}");
        }
    }

    static void Main()
    {
        SolveEquations();
    }
}