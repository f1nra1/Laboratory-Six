#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "main.h"

// Типы и константы
using State = pair<int, int>;
constexpr int V_MAX = 250;  // Безопасное ограничение максимальной скорости
constexpr int INF = -1;     // Константа для обозначения недостижимого состояния

// Вспомогательные функции
vector<int> readTargets() {
    cout << "Введите количество контрольных точек (k): ";
    int k;
    cin >> k;

    cout << "Введите " << k << " целых чисел - координаты контрольных точек в порядке возрастания:" << endl;
    cout << "(каждая точка должна быть больше предыдущей, первая точка > 0)" << endl;

    vector<int> targets(k);
    for (int i = 0; i < k; ++i) {
        cin >> targets[i];

        // Проверка корректности ввода
        if (i > 0 && targets[i] <= targets[i-1]) {
            cout << "Ошибка: каждая следующая точка должна быть больше предыдущей!" << endl;
            exit(1);
        }
        if (i == 0 && targets[i] <= 0) {
            cout << "Ошибка: первая точка должна быть положительным числом!" << endl;
            exit(1);
        }
    }
    return targets;
}


bool isValidSpeed(int speed) {
    return speed > 0 && speed <= V_MAX;
}

// Функция для обработки первого этапа
vector<int> processFirstStage(int target) {
    vector<int> dp(V_MAX + 1, INF);
    
    if (target == 1) {
        dp[0] = 0;
        return dp;
    }

    vector<vector<int>> stage_dist(target + 1, vector<int>(V_MAX + 1, INF));
    queue<State> q;

    // Первый ход всегда начинается с позиции 1 и скорости 1
    constexpr int first_pos = 2;  // 1 + 1
    constexpr int first_speed = 1;
    
    if (first_pos <= target) {
        stage_dist[first_pos][first_speed] = 1;
        q.push({first_pos, first_speed});
    }

    while (!q.empty()) {
        auto [pos, speed] = q.front();
        q.pop();
        int moves = stage_dist[pos][speed];

        for (int dv : {-1, 0, 1}) {
            int next_speed = speed + dv;
            if (!isValidSpeed(next_speed)) continue;

            int next_pos = pos + next_speed;
            if (next_pos <= target && stage_dist[next_pos][next_speed] == INF) {
                stage_dist[next_pos][next_speed] = moves + 1;
                q.push({next_pos, next_speed});
            }
        }
    }

    for (int v = 0; v <= V_MAX; ++v) {
        dp[v] = stage_dist[target][v];
    }
    
    return dp;
}

// Функция для обработки последующих этапов
vector<int> processNextStage(const vector<int>& dp, int prev_pos, int current_pos) {
    vector<vector<int>> stage_dist(current_pos + 1, vector<int>(V_MAX + 1, INF));
    queue<State> q;

    // Мультиисточниковый BFS
    for (int v = 0; v <= V_MAX; ++v) {
        if (dp[v] != INF) {
            stage_dist[prev_pos][v] = dp[v];
            q.push({prev_pos, v});
        }
    }

    while (!q.empty()) {
        auto [pos, speed] = q.front();
        q.pop();
        int moves = stage_dist[pos][speed];

        for (int dv : {-1, 0, 1}) {
            int next_speed = speed + dv;
            if (!isValidSpeed(next_speed)) continue;

            int next_pos = pos + next_speed;
            if (next_pos <= current_pos && 
                (stage_dist[next_pos][next_speed] == INF || 
                 stage_dist[next_pos][next_speed] > moves + 1)) {
                stage_dist[next_pos][next_speed] = moves + 1;
                q.push({next_pos, next_speed});
            }
        }
    }

    vector<int> next_dp(V_MAX + 1, INF);
    if (current_pos < V_MAX) {
        for (int v = 0; v <= V_MAX; ++v) {
            next_dp[v] = stage_dist[current_pos][v];
        }
    }
    
    return next_dp;
}

int findMinimumMoves(const vector<int>& dp) {
    int min_moves = INF;
    for (int moves : dp) {
        if (moves != INF) {
            min_moves = (min_moves == INF) ? moves : min(min_moves, moves);
        }
    }
    return min_moves;
}

void LogicalTasks() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto targets = readTargets();
    auto dp = processFirstStage(targets[0]);

    // Обработка последующих этапов
    for (size_t i = 0; i < targets.size() - 1; ++i) {
        dp = processNextStage(dp, targets[i], targets[i + 1]);
    }

    int result = findMinimumMoves(dp);

    cout << "\nРезультат:\n";
    if (result == INF) {
        cout << "Невозможно достичь всех контрольных точек с заданными ограничениями" << endl;
    } else {
        cout << "Минимальное количество ходов: " << result << endl;
    }
}
