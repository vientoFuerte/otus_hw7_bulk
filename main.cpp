// main.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "bulk.h"

int main(int argc, char* argv[])
{
    // Параметр N - единственный параметр командной строки в виде целого числа.
    size_t N = (argc == 2) ? std::atoi(argv[1]) : 0; // Получаем параметр из командной строки или устанавливаем его в 0 по умолчанию
    // Вектор для хранения команд 
    std::vector<std::string> cmd_pool;
    std::string line;

    for (std::string line; std::getline(std::cin, line);)
     {
         cmd_parser(N, line); 
     } 
}



