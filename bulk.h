#pragma once

#include <vector>
#include <string>

std::string outputBlockStart(std::ofstream& file);
void outputBlockStop(std::ofstream& file);
void blockOutput(std::ofstream& file, std::string cmd, int i, int min, int max);
void cmd_parser(size_t n, const std::vector<std::string>& pool);

