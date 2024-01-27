#include "Statistic.h"
#include <cmath>

Statistic::Statistic(std::shared_ptr<SimExchange> exchange, int initialCapital, int riskfreeRate)
    : exchange(exchange), initialCapital(initialCapital), riskfreeRate(riskfreeRate)
{
    if (exchange->tradeResult.size() != 0)
    {
        GenerateReturnCurve();
        GenerateSharpRatio();
        GenerateTimeSeriesIC();
    }

}

void Statistic::GenerateReturnCurve()
{
    for (const Trade& trade : exchange->tradeResult)
    {
        switch (trade.direction)
        {
        case DirectionType::Buy:
            buyCost += trade.price;
            break;
        case DirectionType::Sell:
            sellReturn += trade.price;
        default:
            break;
        }
        totalFees += trade.fees;
        avgSignal += trade.signal;
        int totalReturn = sellReturn - buyCost - totalFees;
        double returnRate = static_cast<double>(totalReturn) / initialCapital; 
        // auto pair = std::pair<std::string, double>(trade.date, returnRate);
        returnCurve.emplace_back(trade.date, returnRate);
    }
    avgSignal /= returnCurve.size();
}

void Statistic::GenerateSharpRatio()
{
    for (const auto& pair : returnCurve)
    {
        avgReturn += pair.second;
    }
    avgReturn /= returnCurve.size();
    for (const auto& pair : returnCurve)
    {
        variance += std::pow(pair.second - avgReturn, 2);
    }
    variance /= returnCurve.size();
    standardVar = std::sqrt(variance);
    sharpRatio = (avgReturn - riskfreeRate) / standardVar;
    
}

void Statistic::GenerateTimeSeriesIC()
{
    double covOfSignalAndReturn = 0;
    double varianceOfSignal = 0;
    for (int i = 0; i < returnCurve.size(); i++)
    {
        int signal = exchange->tradeResult[i].signal;
        covOfSignalAndReturn += (returnCurve[i].second - avgReturn) * (signal - avgSignal);
        varianceOfSignal += std::pow(signal - avgSignal, 2);
    }
    covOfSignalAndReturn /= returnCurve.size();
    varianceOfSignal /= returnCurve.size();
    timeSeriesIC = covOfSignalAndReturn / (standardVar * std::sqrt(varianceOfSignal));
}