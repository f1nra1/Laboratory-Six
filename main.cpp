#include <iostream>
#include <cstdlib>
#include "main.h"

int main() {
    while (true) {
        int choice;
        cout << "\nВыберите задание:" << endl
             << "1. Матрицы" << endl
             << "2. Шифр AES" << endl
             << "3. СЛАУ" << endl
             << "4. Логические задачи" << endl
             << "5. Выход" << endl
             << "Ваш выбор: ";
        try {
            cin >> choice;
            if (choice == 5) {
                system("clear");
                break;
            }
            Task task = static_cast<Task>(choice);
            switch (task) {
                case Task::Matrix:
                    system("clear");
                    Matrix();
                    break;
                case Task::Aes:
                    system("clear");
                    AES();
                    break;
                case Task::Equations:
                    system("clear");
                    Equations();
                    break;
                case Task::LogicalTasks:
                    system("clear");
                    LogicalTasks();
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
    return 0;
}