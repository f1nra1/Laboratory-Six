#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "main.h"

struct IterationResult {
    vector<double> x;
    bool converged;
    int iterations;
    double error;
};

void printMatrix(const vector<vector<double>>& A, const vector<double>* b = nullptr) {
    cout.setf(ios::fixed);
    cout << setprecision(6);
    for (size_t i = 0; i < A.size(); ++i) {
        for (const double& val : A[i]) {
            cout << setw(12) << val << " ";
        }
        if (b) cout << " | " << setw(12) << (*b)[i];
        cout << '\n';
    }
    cout << '\n';
}

void printVector(const vector<double>& v, const string& title = "") {
    if (!title.empty()) cout << title << '\n';
    cout.setf(ios::fixed);
    cout << setprecision(6);
    for (size_t i = 0; i < v.size(); ++i) {
        cout << "x[" << i << "] = " << v[i] << '\n';
    }
    cout << '\n';
}

void reorderForDiagonalDominance(vector<vector<double>>& A, vector<double>& b) {
    size_t n = A.size();
    for (size_t i = 0; i < n; ++i) {
        size_t maxRow = i;
        double maxRatio = 0;
        
        for (size_t k = i; k < n; ++k) {
            double sum = 0;
            for (size_t j = 0; j < n; ++j) {
                if (k != j) sum += fabs(A[k][j]);
            }
            double ratio = fabs(A[k][i]) / (sum + 1e-10);
            if (ratio > maxRatio) {
                maxRatio = ratio;
                maxRow = k;
            }
        }
        if (maxRow != i) {
            swap(A[i], A[maxRow]);
            swap(b[i], b[maxRow]);
        }
    }
}

IterationResult seidel(vector<vector<double>> A, vector<double> b, 
                      double epsilon = 1e-6, int maxIterations = 100) {
    size_t n = A.size();
    const double omega = 0.5; // параметр релаксации
    vector<double> x(n), xPrev(n);
    
    reorderForDiagonalDominance(A, b);
    
    int iter = 0;
    double error;
    do {
        ++iter;
        xPrev = x;
        
        for (size_t i = 0; i < n; ++i) {
            double sum = b[i];
            for (size_t j = 0; j < n; ++j) {
                if (j < i) sum -= A[i][j] * x[j];
                else if (j > i) sum -= A[i][j] * xPrev[j];
            }
            x[i] = omega * (sum / A[i][i]) + (1 - omega) * xPrev[i];
        }
        
        error = 0;
        for (size_t i = 0; i < n; ++i) {
            error = max(error, fabs(x[i] - xPrev[i]));
        }

        if (iter % 10 == 0) {
            cout << "Итерация " << iter << ", погрешность: " << error << '\n';
        }

        if (iter >= maxIterations) {
            return {x, false, iter, error};
        }
    } while (error > epsilon);

    return {x, true, iter, error};
}

vector<double> luSolve(const vector<vector<double>>& A, const vector<double>& b) {
    size_t n = A.size();
    vector<vector<double>> L(n, vector<double>(n)), U(n, vector<double>(n));
    vector<double> y(n), x(n);

    // LU разложение
    for (size_t i = 0; i < n; ++i) {
        L[i][i] = 1;
        for (size_t j = i; j < n; ++j) {
            U[i][j] = A[i][j];
            for (size_t k = 0; k < i; ++k)
                U[i][j] -= L[i][k] * U[k][j];
        }
        for (size_t j = i + 1; j < n; ++j) {
            L[j][i] = A[j][i];
            for (size_t k = 0; k < i; ++k)
                L[j][i] -= L[j][k] * U[k][i];
            L[j][i] /= U[i][i];
        }
    }

    // Ly = b
    for (size_t i = 0; i < n; ++i) {
        y[i] = b[i];
        for (size_t j = 0; j < i; ++j)
            y[i] -= L[i][j] * y[j];
    }

    // Ux = y
    for (size_t i = n; i-- > 0;) {
        x[i] = y[i];
        for (size_t j = i + 1; j < n; ++j)
            x[i] -= U[i][j] * x[j];
        x[i] /= U[i][i];
    }

    return x;
}

void Equations() {
    const size_t N = 4;
    const vector<vector<double>> A = {
        {-1.13, 0.04, 0.21, -18},
        {0.25, -1.23, 0.14, -0.09},
        {-0.21, 0.14, 0.8, -0.13},
        {0.15, -1.31, 0.06, 0.87}
    };
    const vector<double> b = {-1.24, 0.87, 2.56, -1.13};

    cout << "Решение СЛАУ методом Зейделя и LU-разложением\n\n";
    cout << "Расширенная матрица [A|b]:\n";
    printMatrix(A, &b);

    cout << "=== Метод Зейделя ===\n";
    auto result = seidel(A, b);
    
    if (result.converged) {
        cout << "Решение найдено за " << result.iterations << " итераций\n";
        cout << "Финальная погрешность: " << result.error << "\n";
        printVector(result.x, "Решение методом Зейделя:");
    } else {
        cout << "Метод не сошелся за " << result.iterations << " итераций\n";
        cout << "Текущая погрешность: " << result.error << "\n";
    }

    cout << "\n=== LU-разложение ===\n";
    auto lu_x = luSolve(A, b);
    printVector(lu_x, "Решение методом LU-разложения:");

    cout << "Невязка для метода Зейделя:\n";
    vector<double> r(N);
    for (size_t i = 0; i < N; ++i) {
        r[i] = -b[i];
        for (size_t j = 0; j < N; ++j) {
            r[i] += A[i][j] * result.x[j];
        }
        cout << "r[" << i << "] = " << r[i] << "\n";
    }
}