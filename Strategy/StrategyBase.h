#pragma once
#include "../Public.h"
#include "../SimExchange/SimExchange.h"
#include <string>
#include <memory>


class StrategyBase : public MDSubscriber, public TradeSubscriber
{
public:
    virtual void OnBacktestInit() = 0;

    virtual void OnBacktestStart() = 0;
    virtual void OnBacktestEnd() = 0;
    
    virtual void OnMDUpdate(const DailyData& marketData) = 0;
    virtual void OnRtnTrade(const Trade& trade) = 0;

    virtual ~StrategyBase() {}

public:
    StrategyBase(std::shared_ptr<SimExchange> exchange)
        : exchange(exchange)
    {
    }
protected:
    std::shared_ptr<SimExchange> exchange;
};