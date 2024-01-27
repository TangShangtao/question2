#pragma once
#include "../Public.h"
#include "../SimExchange/SimExchange.h"
#include <utility>

class Statistic
{

public:
    Statistic(std::shared_ptr<SimExchange> exchange, int initialCapital, int riskfreeRate);

private:
    // 根据模拟交易所中的成交记录，计算收益率
    void GenerateReturnCurve();
    void GenerateSharpRatio();
    void GenerateTimeSeriesIC();
public:
    // 日期-收益率
    std::vector<std::pair<std::string, double>> returnCurve;
    // 买入总资金
    double buyCost = 0;
    // 卖出总资金
    double sellReturn = 0;
    // 总手续费
    double totalFees = 0;
    // 平均收益率
    double avgReturn = 0;
    // 收益率方差
    double variance = 0;
    // 收益率标准差
    double standardVar = 0;
    // 夏普比率
    double sharpRatio = 0;
    // 时序IC
    double timeSeriesIC = 0;
    // 因子信号均值
    int avgSignal = 0;
private:
    std::shared_ptr<SimExchange> exchange;
    int initialCapital;
    int riskfreeRate;
};