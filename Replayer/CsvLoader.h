#pragma once

#include "../Public.h"

#include <fstream>

class CsvLoader
{
public:

    bool        LoadFile(const std::string& Path);
    bool        NextRow();
    DailyData   ToDailyData();


    std::string Column = "";
    std::string CurrentRow = "";
private:
    std::vector<std::string> split(const std::string& data);
    std::ifstream ifs_;
};