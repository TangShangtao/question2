#pragma once
#include <cstdlib>
#include <ctime>
#include <memory>

#include "StrategyBase.h"


class TestStrategy : public StrategyBase
{
public:
    TestStrategy(double limit, int initialCapital, std::shared_ptr<SimExchange> exchange)
    : StrategyBase(limit, initialCapital, exchange)
    {
        srand((unsigned)time(nullptr));
    }
    void OnBacktestInit() override final
    {
        StrategyBase::OnBacktestInit();
    }

    void OnBacktestStart() override
    {
        StrategyBase::OnBacktestStart();
    }
    void OnBacktestEnd() override
    {
        StrategyBase::OnBacktestEnd();
    }
    // 策略逻辑：
    // 获取随机数，为0时按收盘价下买单，为1时按收盘价下卖单
    void OnMDUpdate(const DailyData& marketData) override
    {
        StrategyBase::OnMDUpdate(marketData);
        // INFO("{}", marketData.date);
        int tossCoin = rand() % 3;
        if (tossCoin == 0)
        {            
            OrderInsert(marketData.date, DirectionType::Buy, marketData.close, 100, tossCoin);
        }
        else if (tossCoin == 1)
        {
            OrderInsert(marketData.date, DirectionType::Sell, marketData.close, 100, tossCoin);
        }
        else 
        {

        }
    }
    void OnRtnTrade(const Trade& trade) override
    {
        StrategyBase::OnRtnTrade(trade);

    }

};