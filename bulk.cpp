// bulk.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "bulk.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib> // для работы с параметрами командной строки
#include <chrono>
#include <thread>

std::string generateFilename()
{
    // Задержка чтобы имена файлов гарантированно отличались - для отладки 
    // std::this_thread::sleep_for(std::chrono::seconds(1));
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

std::string outputBlockStart(std::ofstream& file)
{
    std::string filename = generateFilename();
    // Открываем файл для записи в бинарном режиме
    file.open(filename, std::ios::binary | std::ios::app);
    bool res = file.is_open();
    if (!res)
    {
        std::cerr << "Failed to open file." << std::endl;
    }

    return filename;
}

void outputBlockStop(std::ofstream& file)
{
    if (file.is_open())
    {
        std::cout << "\n";
        // Закрываем файл
        file.close();
    }
}


void blockOutput(std::ofstream& file, std::string cmd, int i, int min, int max)
{
    if (file.is_open())
    {
        if ((i > min) && (i < (max - 1)))
        {
            std::cout << ", ";
            file << ", ";
        }
        std::cout << cmd;
        file << cmd;
    }
}


void printBlock(const std::vector<std::string>& block, std::ostream& file) {

    std::cout << "bulk : ";
    file << "bulk : ";

    for (size_t j = 0; j < block.size(); ++j) {
        if (j > 0) {
            std::cout << ", ";
            file << ", ";
        }
        file << block[j];
        std::cout << block[j];
    }
}


void cmd_parser(size_t n, const std::vector<std::string>& pool)
{
    if (n <= 0) return;

    int depth = 0;       // глубина вложенности скобок - динамических блоков
    std::ofstream file;  // Объявление без инициализации
    std::string current_filename;
    bool blockOutputStarted = false;
    std::vector<std::string> current_block; // для накопления команд

    for (size_t i = 0; i < pool.size(); i++)
    {
        const std::string& currCmd = pool[i]; // Текущая команда.

        if (currCmd == "{")
        {
            if (depth == 0)//  Динамический блок начался и файл был открыт
            {
                if (!blockOutputStarted)
                {
                    current_filename = outputBlockStart(file);
                    blockOutputStarted = true;
                }
                // Завершаем предыдущий обычный блок, если он не пуст
                if (!current_block.empty() && blockOutputStarted)
                {
                    printBlock(current_block, file);
                    outputBlockStop(file);
                    current_block.clear();
                    blockOutputStarted = false;
                }
            }
            depth++;
        }
        else if (currCmd == "}") // Динамический блок закончился
        {
            depth--;  // Считаем что количество открывающих скобок соответствует количеству закрывающих
            if (depth == 0)  // Завершился текущий динамический блок
            {
                if (blockOutputStarted && !current_block.empty())
                {
                    printBlock(current_block, file);
                    outputBlockStop(file);
                    current_block.clear();
                    blockOutputStarted = false;
                }
            }
        }
        else  // Обычная команда
        {
            // Добавляем команду в текущий блок
            current_block.push_back(currCmd);

            if ((depth == 0) && (current_block.size() >= n))
            {
                if (!blockOutputStarted)
                {
                    current_filename = outputBlockStart(file);

                }
                
                printBlock(current_block, file);
                outputBlockStop(file);
                current_block.clear();
                blockOutputStarted = false;
            }
        }
    }
}


