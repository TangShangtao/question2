#include "SimExchange.h"

SimExchange::SimExchange(double feeRate)
 : feeRate(feeRate) {}

void SimExchange::Subscribe(std::shared_ptr<TradeSubscriber> sub)
{
    tradeSubs.push_back(sub);
}

void SimExchange::OnMDUpdate(const DailyData& marketData)
{
    for (Order& order : orderQue)
    {
        if (order.finished) continue;
        switch (order.direction)
        {
        case DirectionType::Buy:
            if (order.price >= marketData.close)
            {
                order.finished = true;
                auto trade = BuildTradeFromOrder(order, marketData);
                tradeResult.push_back(trade);
                PublishTrade(trade);
            }
            break;
        case DirectionType::Sell:
            if (order.price <= marketData.close)
            {
                order.finished = true;
                auto trade = BuildTradeFromOrder(order, marketData);
                tradeResult.push_back(trade);
                PublishTrade(trade);
            }
            break;
        default:
            ERROR("wrong direction {}", order.direction);
            break;
        }
    }
}


void SimExchange::OrderInsert(const Order& order)
{
    orderQue.push_back(order);
}

double SimExchange::GetFeeRate() 
{
    return feeRate;
}

void SimExchange::PublishTrade(Trade& trade)
{
    for (const auto sub : tradeSubs)
    {
        sub->OnRtnTrade(trade);
    }    
}

Trade SimExchange::BuildTradeFromOrder(const Order& order, const DailyData& marketData)
{
    Trade trade;
    trade.orderID = order.orderID;
    trade.date = marketData.date;
    trade.direction = order.direction;
    trade.price = marketData.close;
    trade.volume = order.volume;
    trade.fees = feeRate * order.price;
    trade.signal = order.signal;
    return trade;
}

// void SimExchange::OnBacktestInit()
// {

// }


// void SimExchange::OnBacktestStart()
// {

// }

// void SimExchange::OnBacktestEnd()
// {

// }