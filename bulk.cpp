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

std::string outputBlockStart(std::ofstream &file)
{ 
    std::string filename = generateFilename();
    // Открываем файл для записи в бинарном режиме
    file.open(filename, std::ios::binary | std::ios::app);
    bool res = file.is_open();
    if (!res)
    {
        std::cerr << "Failed to open file." << std::endl;
    }
    std::cout << "bulk : ";
    file << "bulk : ";

    return filename;
}

void outputBlockStop(std::ofstream &file)
{
    if (file.is_open())
    {
        std::cout << "\n";
      // Закрываем файл
      file.close();
    }
}


void blockOutput(std::ofstream &file, std::string cmd, int i, int min, int max)
{
    if (file.is_open())
    {
      if ((i > min) && (i< (max-1)))
      {
          std::cout << ", ";
          file << ", ";
      }    
      std::cout << cmd;
      file << cmd;
    }
}

void cmd_parser(size_t n, const std::vector<std::string> &pool)
{
    if (n <= 0) return;
    
    int depth = 0;       // глубина вложенности скобок - динамических блоков
    bool blockOutputStarted = false;
    std::ofstream file;  // Объявление без инициализации
    std::string current_filename; 

    for (size_t currCmd = 0; currCmd < pool.size(); currCmd += n)
    {
        if(!blockOutputStarted) {
            current_filename = outputBlockStart(file);
        }
        
        // Определяем конец текущего блока
        size_t end = std::min(currCmd + n, pool.size());
        
        for (size_t i = currCmd; i < end; i++)
        {
            if (pool[i] == "{")  // начался динамический блок
            {
               if(depth==0)  // для первого мы создаем новый файл, для вложенных игнорируем.
               {
                  outputBlockStop(file);
                  
                  current_filename = outputBlockStart(file);
                  
                }
              depth++;
              continue;       
            }
            else if (pool[i] == "}") // закончился динамический блок
            {
                if(depth == 1){
                  outputBlockStop(file);
                }
                depth--;
                
                continue;
            }
            
            blockOutput(file, pool[i], i, currCmd, currCmd + n);
         
        }
         if(depth == 0 && blockOutputStarted)  {
              outputBlockStop(file);   
         }
    
    }
}
    



