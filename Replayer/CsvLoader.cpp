#include "CsvLoader.h"

bool CsvLoader::LoadFile(const std::string& Path)
{
    ifs_ = std::ifstream(Path);
    if (!ifs_.is_open())
    {
        ERROR("CsvLoader: open file {} failed", Path);
        return false;
    }
    NextRow();
    std::getline(ifs_, Column);
    return true;
}
// 到eof返回false, 否则返回true
bool CsvLoader::NextRow()
{
    if (ifs_.eof()) return false;
    std::getline(ifs_, CurrentRow);
    if (CurrentRow.size() == 0) return false;
    return true;
}

DailyData CsvLoader::ToDailyData()
{
    DailyData data;
    auto fieldVec = split(CurrentRow);
    data.date = fieldVec[0];
    data.open = std::stod(fieldVec[1]);
    data.close = std::stod(fieldVec[2]);
    data.high = std::stod(fieldVec[3]);
    data.low = std::stod(fieldVec[4]);
    return data;
}

std::vector<std::string> CsvLoader::split(const std::string& str)
{
    std::vector<std::string> ret;
    size_t start, pos;
    start = 0;
    do
    {
        pos = str.find_first_of(",", start);
        if (pos == start)
        {
            ret.emplace_back("");
            start = pos + 1;
        }
        else if (pos == std::string::npos)
        {
            ret.emplace_back( str.substr(start) );
            break;
        }
        else
        {
            ret.emplace_back( str.substr(start, pos - start) );
            start = pos + 1;
        }

    } while (pos != std::string::npos);
    return ret;    
}