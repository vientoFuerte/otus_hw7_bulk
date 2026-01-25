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
     std::this_thread::sleep_for(std::chrono::seconds(2));
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
         std::cout << std::endl; 

        // Закрываем файл
        file.close();
    }
}


void printCmd(const std::string& currCmd, std::ostream& file, bool &first_command ) {

        if (!first_command) {
            std::cout << ", ";
            file << ", ";
        }
        else{
            std::cout << "bulk : ";
            file << "bulk : ";
            first_command = false;  // После вывода "bulk : " меняем флаг
        }
        
        file << currCmd;
        std::cout << currCmd;
}


void cmd_parser(size_t n, const std::string& currCmd)
{
    if (n <= 0) return;

    static int depth = 0;       // глубина вложенности скобок - динамических блоков
    static std::ofstream file;  // Объявление без инициализации
    static std::string current_filename;
    static bool blockOutputStarted = false;
    static std::vector<std::string> current_block; // для накопления команд
    static bool first_command = true;  // для вывода запятых

    if (currCmd == "{")
    {
        if (depth == 0)//  Динамический блок начался и файл был открыт
        {
            if (!blockOutputStarted)
            {
                 current_filename = outputBlockStart(file);
                 blockOutputStarted = true; 
                 first_command = true;
            }
                
            if (!current_block.empty()) // Завершаем предыдущий обычный блок, если он не пуст
            {
                for (const auto& cmd : current_block) {
                  printCmd(cmd, file, first_command);
                }

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
                // Выводим все накопленные команды
                for (const auto& cmd : current_block){
                   printCmd(cmd, file, first_command);
                }
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

        if ((depth == 0) && (current_block.size() >= n)) // Не в динамическом блоке и накопили достаточно
        {
            if (!blockOutputStarted)
            {
                current_filename = outputBlockStart(file);
                blockOutputStarted = true;
                first_command = true;
            }
            
             // Выводим N команд
            for (const auto& cmd : current_block)
            {
                printCmd(cmd, file, first_command);
            }
            outputBlockStop(file);
            current_block.clear();
            blockOutputStarted = false;
        }
    }

// Вывод оставшихся команд после обработки всех входных данных
if (depth == 0 && !current_block.empty()) {
    // Только если мы не внутри динамического блока и есть команды
    if (!blockOutputStarted) {
        current_filename = outputBlockStart(file);
        blockOutputStarted = true;
        first_command = true;
    }
    
    for (const auto& cmd : current_block)
    {
        printCmd(cmd, file, first_command);
    }
    outputBlockStop(file);
}
}

