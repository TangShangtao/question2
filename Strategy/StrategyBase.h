#pragma once
#include "../Public.h"
#include "../SimExchange/SimExchange.h"
#include <string>
#include <memory>


class StrategyBase : public MDSubscriber, public TradeSubscriber
{
public:
    void OnBacktestInit() override;

    void OnBacktestStart() override;
    void OnBacktestEnd() override;
    
    void OnMDUpdate(const DailyData& marketData) override;
    void OnRtnTrade(const Trade& trade) override;

    virtual ~StrategyBase() {}

public:
    // 涨跌幅限制、初始资金、模拟交易所接口
    StrategyBase(double limit, int initialCapital, std::shared_ptr<SimExchange> exchange)
        : limit(limit), initialCapital(initialCapital), currentCapital(initialCapital), exchange(exchange)
    {
    }
    // 资金不足、股票不足、超出涨跌停限制时，无法下单
    bool OrderInsert(const std::string& date, DirectionType direction, double price, double volume, int signal);
    int currentCapital;
protected:
    double limit;
    int initialCapital;
    // int currentCapital;
    int currentStock = 0;
    uint32_t currentOrderID = 0;
private:
    std::shared_ptr<SimExchange> exchange;
    double upLimit = 0;     // 涨幅限制
    double downLimit = 0;   // 跌幅限制
    double todayClose = 0;  // 今日收盘价
};