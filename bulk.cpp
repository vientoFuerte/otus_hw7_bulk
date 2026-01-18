// bulk.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "bulk.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib> // для работы с параметрами командной строки



// Функция argc содержит количество аргументов командной строки, а массив argv содержит сами аргументы.


void static_block()
{

}

std::string generateFilename()
{
    // Получаем текущее время
    std::time_t now = std::time(nullptr);
    std::tm timeinfo;
    localtime_r(&now, &timeinfo);

    // Формируем имя файла
    std::string filename = "bulk";
    filename += std::to_string(timeinfo.tm_mday);
    filename += std::to_string(timeinfo.tm_mon + 1);
    filename += std::to_string(timeinfo.tm_year + 1900);
    filename += std::to_string(timeinfo.tm_hour);
    filename += std::to_string(timeinfo.tm_min);
    filename += std::to_string(timeinfo.tm_sec);
    filename += ".log";

    return filename;
}

void cmd_parser(size_t n, const std::vector<std::string> &pool)
{
    static size_t currCmd = 0;
    while (currCmd < pool.size())
    {
        // Открываем файл для записи в бинарном режиме
        std::ofstream file(generateFilename(), std::ios::binary | std::ios::app);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file." << std::endl;
            continue;
        }

        std::cout << "bulk :" << "\t";
        file << "bulk :" << "\t";
        for (size_t i = currCmd; i < (currCmd + n) && i < pool.size(); ++i)
        {
            std::cout << pool[i] << "\t";
            file << pool[i] << "\t";
           
        }

        // Закрываем файл
        file.close();

        currCmd += 3;
        std::cout << "\n";


            
    }
}
    




