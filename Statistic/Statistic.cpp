#include "Statistic.h"
#include <cmath>
#include <fstream>
#include <iostream>
Statistic::Statistic(const std::string& code, std::shared_ptr<SimExchange> exchange, int initialCapital, int riskfreeRate)
    : code(code), exchange(exchange), initialCapital(initialCapital), riskfreeRate(riskfreeRate)
{
    if (exchange->tradeResult.size() != 0)
    {
        GenerateReturnCurve();
        GenerateSharpRatioAndDrawDown();
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
        returnCurve.emplace_back(trade.date, returnRate);
    }
    avgSignal /= returnCurve.size();
}

void Statistic::GenerateSharpRatioAndDrawDown()
{
    double maxReturn = DBL_MIN;
    for (const auto& pair : returnCurve)
    {
        double returnRate = pair.second;
        maxReturn = std::max(returnRate, maxReturn);
        double currDrawDown = maxReturn - returnRate;
        maxDrawDown = std::max(currDrawDown, maxDrawDown);
        avgReturn += returnRate;
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

void Statistic::ToCsv()
{
    std::string orderFile = code + "-orders.csv";
    std::string tradeFile = code + "-trades.csv";
    std::string statisticFile = code + "-statistics.csv";
    
    std::ofstream outputOrder(orderFile); 
    if (outputOrder.is_open()) 
    {
        outputOrder << "orderID, date, direction, price, volume, signal, finished" << std::endl;  
        for (const auto& order : exchange->orderQue)
        {
            outputOrder << 
            order.orderID << "," << order.date << "," << 
            order.direction << "," <<
            order.price << "," << order.volume << "," << 
            order.signal << "," << order.finished 
            << std::endl;
        }
        outputOrder.close();  
    } 
    else ERROR("open {} failed", orderFile);

    std::ofstream outputTrade(tradeFile); 
    if (outputTrade.is_open()) 
    {
        outputTrade << "orderID, date, direction, price, volume, fees, signal" << std::endl;  
        for (const auto& trade : exchange->tradeResult)
        {
            outputTrade << 
            trade.orderID << "," << trade.date << "," <<
            trade.direction << "," << trade.price << "," << 
            trade.volume << "," << trade.fees << "," << trade.signal
            << std::endl;
        }
        outputTrade.close();  
    } 
    else ERROR("open {} failed", tradeFile);

    std::ofstream outputStatistic(statisticFile); 
    if (outputStatistic.is_open()) 
    {
        outputStatistic << "maxDrawDown, buyCost, sellReturn, totalFees, avgReturn, variance,\
        standardVar, sharpRatio, timeSeriesIC, avgSignal" << std::endl;
        outputStatistic << 
        maxDrawDown << "," << buyCost << "," <<
        sellReturn << "," << totalFees << "," << 
        avgReturn << ","  << variance << "," <<
        standardVar << "," << sharpRatio << "," << 
        timeSeriesIC << "," << avgSignal << std::endl;
        
        outputStatistic << std::endl;
        outputStatistic << "date, returnRate" << std::endl;
        for (const auto& pair : returnCurve)
        {
            outputStatistic << pair.first << "," << pair.second << std::endl;
        }
        outputStatistic.close();  
    } 
    else ERROR("open {} failed", statisticFile);
}

