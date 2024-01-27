#include "../Strategy/TestStrategy.h"
#include "../Replayer/MDReplayer.h"
#include "../SimExchange/SimExchange.h"
#include "../Statistic/Statistic.h"
#include <iostream>
#include <memory>
#include <unistd.h>
int main()
{
    char currentPath[FILENAME_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) == nullptr) 
    {
        ERROR("Fail to get cwd");
        return 0;
    } 
    std::string DataDir = currentPath;
    DataDir += "/../TestData";
    MDReplayer replayer(DataDir, "002998");
    auto exchange = std::make_shared<SimExchange>(0.001);
    auto testStra = std::make_shared<TestStrategy>(exchange);
    replayer.Subscribe(exchange);
    replayer.Subscribe(testStra);
    exchange->Subscribe(testStra);
    bool res = replayer.Init();
    if (!res) return 0;
    replayer.Run();
    Statistic statistic(exchange, 10000, 0.02);
    INFO("SharpRatio: {}", statistic.sharpRatio);
    INFO("avgReturn: {}", statistic.avgReturn);
}