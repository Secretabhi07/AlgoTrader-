#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include <unordered_map>
#include <deque>
#include <numeric>
#include <iostream>
#include <iomanip>
#include "market_data.h"

enum class OrderType { BUY, SELL };

class Order {
public:
    int orderId;
    std::string symbol;
    OrderType type;
    int quantity;
    double price;
    Order();
    Order(int id, const std::string& sym, OrderType t, int qty, double p)
        : orderId(id), symbol(sym), type(t), quantity(qty), price(p) {}
};

class Strategy {
protected:
    std::string name;
    std::unordered_map<std::string, double> positions;
    double cash;
    
    virtual void generateOrder(const std::string& symbol, OrderType type, int quantity, double price);
    
public:
    Strategy(const std::string& strategyName, double initialCash);
    virtual ~Strategy() = default;
    
    virtual void onMarketData(const MarketData& data) = 0;
    virtual void onOrderFilled(const Order& order) = 0;
    virtual void onTimer() = 0;
    
    const std::string& getName() const { return name; }
    double getCash() const { return cash; }
    double getPosition(const std::string& symbol) const;
};

class MovingAverageCrossover : public Strategy {
private:
    std::string symbol;
    int shortPeriod, longPeriod;
    std::deque<double> prices;
    double shortMA = 0.0, longMA = 0.0;
    bool prevCrossAbove = false;
    
    void updateMovingAverages();
    
protected:
    // Make generateOrder virtual so it can be overridden in tests
    virtual void generateOrder(const std::string& symbol, OrderType type, int quantity, double price) override;
    
public:
    MovingAverageCrossover(const std::string& sym, int shortP, int longP, double initialCash);
    
    void onMarketData(const MarketData& data) override;
    void onOrderFilled(const Order& order) override;
    void onTimer() override;
};

#endif // STRATEGY_H