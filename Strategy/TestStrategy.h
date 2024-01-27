#pragma once
#include <cstdlib>
#include <ctime>
#include <memory>

#include "StrategyBase.h"


class TestStrategy : public StrategyBase
{
public:
    TestStrategy(std::shared_ptr<SimExchange> exchange)
    : StrategyBase(exchange)
    {
        srand((unsigned)time(nullptr));
    }
    void OnBacktestInit() override
    {

    }

    void OnBacktestStart() override
    {

    }
    void OnBacktestEnd() override
    {

    }
    
    void OnMDUpdate(const DailyData& marketData) override
    {
        INFO("{}", marketData.date);
        int tossCoin = rand() % 3;
        if (tossCoin == 0)
        {
            Order order;
            
            order.direction = DirectionType::Buy;
            order.price = 1.5;
            order.volume = 100;
            order.signal = tossCoin;
            exchange->OrderInsert(order);
        }
        else if (tossCoin == 1)
        {
            Order order;
            order.direction = DirectionType::Sell;
            order.price = 1.5;
            order.volume = 100;
            order.signal = tossCoin;
            exchange->OrderInsert(order);
        }
        else 
        {

        }
    }
    void OnRtnTrade(const Trade& trade) override
    {

    }

};