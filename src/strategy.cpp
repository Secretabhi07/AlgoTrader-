#include "strategy.h"

Strategy::Strategy(const std::string& strategyName, double initialCash) 
    : name(strategyName), cash(initialCash) {}

void Strategy::generateOrder(const std::string& symbol, OrderType type, int quantity, double price) {
    std::cout << "Generated Order: " << symbol 
              << (type == OrderType::BUY ? " BUY " : " SELL ")
              << quantity << " @ $" << price << std::endl;
}

double Strategy::getPosition(const std::string& symbol) const {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second : 0.0;
}

MovingAverageCrossover::MovingAverageCrossover(const std::string& sym, int shortP, int longP, double initialCash)
    : Strategy("MA_Crossover", initialCash), symbol(sym), shortPeriod(shortP), longPeriod(longP) {}

void MovingAverageCrossover::updateMovingAverages() {
    if (prices.size() >= static_cast<size_t>(shortPeriod)) {
        shortMA = std::accumulate(prices.end() - shortPeriod, prices.end(), 0.0) / shortPeriod;
    }
    if (prices.size() >= static_cast<size_t>(longPeriod)) {
        longMA = std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
    }
}

void MovingAverageCrossover::onMarketData(const MarketData& data) {
    if (data.symbol != symbol) return;
    
    prices.push_back(data.last);
    if (prices.size() > static_cast<size_t>(longPeriod)) {
        prices.pop_front();
    }
    
    updateMovingAverages();
    
    if (prices.size() >= static_cast<size_t>(longPeriod)) {
        bool currentCrossAbove = (shortMA > longMA);
        
        if (currentCrossAbove && !prevCrossAbove) {
            generateOrder(symbol, OrderType::BUY, 100, data.ask);
        } else if (!currentCrossAbove && prevCrossAbove) {
            generateOrder(symbol, OrderType::SELL, 100, data.bid);
        }
        
        prevCrossAbove = currentCrossAbove;
        
        std::cout << "MA Values - Short: " << shortMA << ", Long: " << longMA << std::endl;
    }
}

void MovingAverageCrossover::onOrderFilled(const Order& order) {
    std::cout << "Order filled: " << order.symbol << std::endl;
}

void MovingAverageCrossover::onTimer() {
    // Timer-based logic if needed
}
// In strategy.cp
Order::Order()
    : orderId(0), symbol(""), type(OrderType::BUY), quantity(0), price(0.0) {}