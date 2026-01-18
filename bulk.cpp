// bulk.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "bulk.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib> // для работы с параметрами командной строки
#include <chrono>
#include <thread>



// Функция argc содержит количество аргументов командной строки, а массив argv содержит сами аргументы.


void static_block()
{

}

std::string generateFilename()
{
    // Задержка чтобы имена файлов гарантированно отличались
    std::this_thread::sleep_for(std::chrono::seconds(1));
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
    if (n <= 0) return;

    for (size_t currCmd = 0; currCmd < pool.size(); currCmd += n)
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
        // Определяем конец текущего блока
        size_t end = std::min(currCmd + n, pool.size());
        
        for (size_t i = currCmd; i < end; i++)
        {
            std::cout << pool[i];
            file << pool[i];
            if (i< (end-1))
            {
                std::cout << ", ";
                file << ", ";
            }          
        }
        std::cout << "\n";
        // Закрываем файл
        file.close();
    
    }
}
    



