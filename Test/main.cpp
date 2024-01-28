#include "../Strategy/TestStrategy.h"
#include "../Replayer/MDReplayer.h"
#include "../SimExchange/SimExchange.h"
#include "../Statistic/Statistic.h"
#include <iostream>
#include <memory>
#include <unistd.h>
int main()
{
    // 在此指定回测的股票、手续费率、涨跌停限制、初始资金、无风险利率
    char currentPath[FILENAME_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) == nullptr) 
    {
        ERROR("Fail to get cwd");
        return 0;
    } 
    std::string DataDir = currentPath;
    DataDir += "/../TestData";
    std::string code = "000001";        // 股票
    double feeRate = 0.001;             // 手续费率
    double limit = 0.1;                 // 涨跌停限制
    int initialCapital = 10000;         // 初始资金
    double riskfee = 0.02;              // 无风险利率
    Logger::Init(Logger::level_enum::debug);


    MDReplayer replayer(DataDir, code);
    auto exchange = std::make_shared<SimExchange>(feeRate);
    auto testStra = std::make_shared<TestStrategy>(limit, initialCapital, exchange);
    replayer.Subscribe(exchange);
    replayer.Subscribe(testStra);
    exchange->Subscribe(testStra);
    bool res = replayer.Init();
    if (!res) return 0;
    replayer.Run();
    Statistic statistic(code, exchange, initialCapital, riskfee);
    INFO("SharpRatio: {}", statistic.sharpRatio);
    INFO("avgReturn: {}", statistic.avgReturn);
    INFO("timeseriesIC: {}", statistic.timeSeriesIC);
    INFO("currentCapital: {}", testStra->currentCapital);
    statistic.ToCsv();
}