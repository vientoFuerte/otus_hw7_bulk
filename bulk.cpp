#include "bulk.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

std::string generateFilename()
{
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    std::time_t now = std::time(nullptr);
    std::tm timeinfo;
    localtime_r(&now, &timeinfo);

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
    file.open(filename, std::ios::binary | std::ios::app);
    if (!file.is_open())
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
        file.close();
    }
}


void printBlock(const std::vector<std::string>& block, std::ostream& file) {
    std::cout << "bulk : ";
    file << "bulk : ";
    
    for (size_t i = 0; i < block.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
            file << ", ";
        }
        std::cout << block[i];
        file << block[i];
    }
}

void cmd_parser(size_t n, const std::string& currCmd)
{
    if (n <= 0) return;

    static int depth = 0;
    static std::ofstream file;
    static std::string current_filename;
    static bool blockOutputStarted = false;
    static std::vector<std::string> current_block;
      
    if (currCmd == "{")
    {
        if (depth == 0)
        { 
            if (!current_block.empty())
            {
                if (!blockOutputStarted)
                {
                     current_filename = outputBlockStart(file);
                     blockOutputStarted = true;
                }
                printBlock(current_block, file);
                outputBlockStop(file);
                current_block.clear();
                blockOutputStarted = false;
            }
        }
        depth++;
    }
    else if (currCmd == "}")
    {
        if (depth > 0) {
            depth--;
            if (depth == 0)
            {
                if (!current_block.empty())
                {
                    if (!blockOutputStarted)
                    {
                        current_filename = outputBlockStart(file);
                        blockOutputStarted = true;
                    }
                    printBlock(current_block, file);   
                    outputBlockStop(file);
                    current_block.clear();
                    blockOutputStarted = false;
                }
            }
        }
    }
    else if (!currCmd.empty())
    {
        current_block.push_back(currCmd);

        if ((depth == 0) && (current_block.size() >= n))
        {
            if (!blockOutputStarted)
            {
                current_filename = outputBlockStart(file);
                blockOutputStarted = true;
            }
            
            printBlock(current_block, file);
            outputBlockStop(file);
            current_block.clear();
            blockOutputStarted = false;
        }
    }    
    else if (currCmd.empty()) { // Если строка пустая - это конец ввода команд
        if (depth == 0 && !current_block.empty()) {
            if (!blockOutputStarted) {
                current_filename = outputBlockStart(file);
                blockOutputStarted = true;
            }
            printBlock(current_block, file);
            outputBlockStop(file);
            current_block.clear();
            blockOutputStarted = false;
        }
        return;
    }

}
