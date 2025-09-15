#ifndef MAIN_H
#define MAIN_H
using namespace std;

enum class Task{
    Matrix = 1,
    Aes,
    Equations,
    LogicalTasks
};

void Matrix();
void AES();
void Equations();
void LogicalTasks();

#endif