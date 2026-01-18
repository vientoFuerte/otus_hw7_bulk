// main.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "bulk.h"


int main(int argc, char* argv[])
{
    // Параметр N - единственный параметр командной строки в виде целого числа.
    size_t N = (argc == 2) ? std::atoi(argv[1]) : 0; // Получаем параметр из командной строки или устанавливаем его в 0 по умолчанию
    /*

    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }*/
    
    // Вектор для хранения команд 
    std::vector<std::string> cmd_pool;
    std::string line;

   /* for (std::string line; std::getline(std::cin, line);)
    {

        cmd_pool.push_back(line);
        std::cout << line<< "\n";
    } */

    /* подыгрыш приема */

    N = 3;
    cmd_pool.push_back("cmd1");
    cmd_pool.push_back("cmd2");
    cmd_pool.push_back("cmd3");
    cmd_pool.push_back("cmd4");
    cmd_pool.push_back("cmd5");

    /* Обрабатывать команды из пула по N штук. */
    cmd_parser(N, cmd_pool);

    /*for (const auto& cmd : cmd_pool) {
        std::cout << cmd << std::endl;
    }*/


 
}

