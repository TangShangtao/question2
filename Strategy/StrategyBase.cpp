#include "StrategyBase.h"


void StrategyBase::OnBacktestInit()
{
    INFO("backtest init");
}
void StrategyBase::OnBacktestStart()
{
    INFO("backtest start");
}
void StrategyBase::OnBacktestEnd()
{
    INFO("backtest end");
}  
void StrategyBase::OnMDUpdate(const DailyData& marketData)
{
    double lastClose = todayClose;
    todayClose = marketData.close;
    upLimit = (1 + limit) * lastClose;
    downLimit = (1 - limit) * lastClose;
}
void StrategyBase::OnRtnTrade(const Trade& trade)
{
    switch (trade.direction)
    {
    case DirectionType::Buy:
        currentCapital -= trade.price * trade.volume;
        currentStock += trade.volume;
        break;
    case DirectionType::Sell:
        currentCapital += trade.price * trade.volume;
        currentStock -= trade.volume;
        break;
    default:
        break;
    }
}

bool StrategyBase::OrderInsert(const std::string& date, DirectionType direction, double price, double volume, int signal)
{
    switch (direction)
    {
    case DirectionType::Buy:
        if (price * volume >= currentCapital) return false;
        if (price > upLimit) return false;
        break;
    case DirectionType::Sell:
        if (volume >= currentStock) return false;
        if (price < downLimit) return false;
        break;
    default:
        break;
    }
    currentOrderID++;
    Order order;
    order.date = date;
    order.direction = direction;
    order.orderID = currentOrderID;
    order.price = price;
    order.volume = volume;
    order.signal = signal;
    exchange->OrderInsert(order);
    return true;
}