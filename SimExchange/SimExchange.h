#pragma once
#include <vector>
#include <memory>
#include "../Public.h"

class SimExchange : public MDSubscriber
{
public:
    // 指定交易手续费率
    SimExchange(double feeRate);
    void Subscribe(std::shared_ptr<TradeSubscriber> sub);
    void OnBacktestInit() override {};
    void OnBacktestStart() override {};
    void OnBacktestEnd() override {};
    
    // 接收最新一日行情，用收盘价撮合订单
    void OnMDUpdate(const DailyData& marketData) override;

public:
    void OrderInsert(const Order& order);
    // bool OrderCancel()
    double GetFeeRate();
private:
    void PublishTrade(Trade& trade);
    Trade BuildTradeFromOrder(const Order& order, const DailyData& marketData);
    // void OrderCancel()

private:
    friend class Statistic;
    std::vector<Order> orderQue;
    std::vector<Trade> tradeResult;
    std::vector<std::shared_ptr<TradeSubscriber>> tradeSubs;
    double feeRate;
};