using System;
using System.Collections.Generic;
using System.Linq;

public class Program
{
    // Типы и константы
    private record struct State(int Position, int Speed);
    private const int V_MAX = 250;  // Безопасное ограничение максимальной скорости
    private const int INF = -1;     // Константа для обозначения недостижимого состояния

    // Вспомогательные функции
    private static List<int> ReadTargets()
    {
        Console.Write("Введите количество контрольных точек (k): ");
        int k = int.Parse(Console.ReadLine()!);

        Console.WriteLine($"Введите {k} целых чисел - координаты контрольных точек в порядке возрастания:");
        Console.WriteLine("(каждая точка должна быть больше предыдущей, первая точка > 0)");

        var targets = new List<int>(k);
        for (int i = 0; i < k; i++)
        {
            int point = int.Parse(Console.ReadLine()!);

            // Проверка корректности ввода
            if (i > 0 && point <= targets[i - 1])
            {
                Console.WriteLine("Ошибка: каждая следующая точка должна быть больше предыдущей!");
                Environment.Exit(1);
            }
            if (i == 0 && point <= 0)
            {
                Console.WriteLine("Ошибка: первая точка должна быть положительным числом!");
                Environment.Exit(1);
            }
            targets.Add(point);
        }
        return targets;
    }

    private static bool IsValidSpeed(int speed) => speed > 0 && speed <= V_MAX;

    // Функция для обработки первого этапа
    private static int[] ProcessFirstStage(int target)
    {
        var dp = Enumerable.Repeat(INF, V_MAX + 1).ToArray();

        if (target == 1)
        {
            dp[0] = 0;
            return dp;
        }

        var stageDist = new int[target + 1][];
        for (int i = 0; i <= target; i++)
        {
            stageDist[i] = Enumerable.Repeat(INF, V_MAX + 1).ToArray();
        }

        var queue = new Queue<State>();

        // Первый ход всегда начинается с позиции 1 и скорости 1
        const int firstPos = 2;  // 1 + 1
        const int firstSpeed = 1;

        if (firstPos <= target)
        {
            stageDist[firstPos][firstSpeed] = 1;
            queue.Enqueue(new State(firstPos, firstSpeed));
        }

        while (queue.Count > 0)
        {
            var (pos, speed) = queue.Dequeue();
            int moves = stageDist[pos][speed];

            foreach (int dv in new[] { -1, 0, 1 })
            {
                int nextSpeed = speed + dv;
                if (!IsValidSpeed(nextSpeed)) continue;

                int nextPos = pos + nextSpeed;
                if (nextPos <= target && stageDist[nextPos][nextSpeed] == INF)
                {
                    stageDist[nextPos][nextSpeed] = moves + 1;
                    queue.Enqueue(new State(nextPos, nextSpeed));
                }
            }
        }

        for (int v = 0; v <= V_MAX; v++)
        {
            dp[v] = stageDist[target][v];
        }

        return dp;
    }

    // Функция для обработки последующих этапов
    private static int[] ProcessNextStage(int[] dp, int prevPos, int currentPos)
    {
        var stageDist = new int[currentPos + 1][];
        for (int i = 0; i <= currentPos; i++)
        {
            stageDist[i] = Enumerable.Repeat(INF, V_MAX + 1).ToArray();
        }

        var queue = new Queue<State>();

        // Мультиисточниковый BFS
        for (int v = 0; v <= V_MAX; v++)
        {
            if (dp[v] != INF)
            {
                stageDist[prevPos][v] = dp[v];
                queue.Enqueue(new State(prevPos, v));
            }
        }

        while (queue.Count > 0)
        {
            var (pos, speed) = queue.Dequeue();
            int moves = stageDist[pos][speed];

            foreach (int dv in new[] { -1, 0, 1 })
            {
                int nextSpeed = speed + dv;
                if (!IsValidSpeed(nextSpeed)) continue;

                int nextPos = pos + nextSpeed;
                if (nextPos <= currentPos &&
                    (stageDist[nextPos][nextSpeed] == INF ||
                     stageDist[nextPos][nextSpeed] > moves + 1))
                {
                    stageDist[nextPos][nextSpeed] = moves + 1;
                    queue.Enqueue(new State(nextPos, nextSpeed));
                }
            }
        }

        var nextDp = Enumerable.Repeat(INF, V_MAX + 1).ToArray();
        if (currentPos < V_MAX)
        {
            for (int v = 0; v <= V_MAX; v++)
            {
                nextDp[v] = stageDist[currentPos][v];
            }
        }

        return nextDp;
    }

    private static int FindMinimumMoves(int[] dp)
    {
        int minMoves = INF;
        foreach (int moves in dp)
        {
            if (moves != INF)
            {
                minMoves = (minMoves == INF) ? moves : Math.Min(minMoves, moves);
            }
        }
        return minMoves;
    }

    public static void LogicalTasks()
    {
        var targets = ReadTargets();
        var dp = ProcessFirstStage(targets[0]);

        // Обработка последующих этапов
        for (int i = 0; i < targets.Count - 1; i++)
        {
            dp = ProcessNextStage(dp, targets[i], targets[i + 1]);
        }

        int result = FindMinimumMoves(dp);

        Console.WriteLine("\nРезультат:");
        if (result == INF)
        {
            Console.WriteLine("Невозможно достичь всех контрольных точек с заданными ограничениями");
        }
        else
        {
            Console.WriteLine($"Минимальное количество ходов: {result}");
        }
    }

    public static void Main()
    {
        LogicalTasks();
    }
}