#pragma once
#include "../Public.h"
#include "CsvLoader.h"
#include <string>
#include <vector>
#include <memory>

class MDReplayer
{
public:
    MDReplayer(const std::string& dataPath, const std::string& code);
    bool Init();
    void Subscribe(std::shared_ptr<MDSubscriber> sub);
    
    void Run();

private:
    std::string csvPath;
    CsvLoader loader;
    std::vector<std::shared_ptr<MDSubscriber>> MDSubs;
};