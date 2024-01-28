#pragma once
#include "../Public.h"
#include "../SimExchange/SimExchange.h"
#include <utility>
#include <cfloat>
class Statistic
{

public:
    // 指定初始资金、无风险利率
    Statistic(const std::string& code, std::shared_ptr<SimExchange> exchange, int initialCapital, int riskfreeRate);
    void ToCsv();
private:
    // 根据模拟交易所中的成交记录，计算收益率
    void GenerateReturnCurve();
    // 根据收益率序列，计算夏普比率和最大回撤
    void GenerateSharpRatioAndDrawDown();
    // 根据模拟交易所中的成交记录(因子信号)和收益率序列，计算时序IC
    void GenerateTimeSeriesIC();
public:
    // 日期-收益率
    std::vector<std::pair<std::string, double>> returnCurve;
    // 最大回撤
    double maxDrawDown = DBL_MIN;
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
    std::string code;
    std::shared_ptr<SimExchange> exchange;
    int initialCapital;
    int riskfreeRate;
};