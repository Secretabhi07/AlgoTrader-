#include "strategy.h"

Strategy::Strategy(const std::string& strategyName, double initialCash) 
    : name(strategyName), cash(initialCash) {}

void Strategy::generateOrder(const std::string& symbol, OrderType type, int quantity, double price) {
    std::cout << "Generated Order: " << symbol 
              << (type == OrderType::BUY ? " BUY " : " SELL ")
              << quantity << " @ $" << std::fixed << std::setprecision(2) << price << std::endl;
}

double Strategy::getPosition(const std::string& symbol) const {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second : 0.0;
}

MovingAverageCrossover::MovingAverageCrossover(const std::string& sym, int shortP, int longP, double initialCash)
    : Strategy("MA_Crossover", initialCash), symbol(sym), shortPeriod(shortP), longPeriod(longP) {}

void MovingAverageCrossover::updateMovingAverages() {
    // Calculate short moving average
    if (prices.size() >= static_cast<size_t>(shortPeriod)) {
        shortMA = std::accumulate(prices.end() - shortPeriod, prices.end(), 0.0) / shortPeriod;
    }
    
    // Calculate long moving average - FIXED: use correct period
    if (prices.size() >= static_cast<size_t>(longPeriod)) {
        longMA = std::accumulate(prices.end() - longPeriod, prices.end(), 0.0) / longPeriod;
    }
}

// This is the key fix - implement the virtual function
void MovingAverageCrossover::generateOrder(const std::string& symbol, OrderType type, int quantity, double price) {
    // Call the parent implementation
    Strategy::generateOrder(symbol, type, quantity, price);
}

void MovingAverageCrossover::onMarketData(const MarketData& data) {
    if (data.symbol != symbol) return;
    
    prices.push_back(data.last);
    if (prices.size() > static_cast<size_t>(longPeriod)) {
        prices.pop_front();
    }
    
    updateMovingAverages();
    
    // Only start trading when we have enough data for both averages
    if (prices.size() >= static_cast<size_t>(longPeriod)) {
        bool currentCrossAbove = (shortMA > longMA);
        
        // Check for crossover signals
        if (currentCrossAbove && !prevCrossAbove) {
            // Short MA crossed above Long MA - BUY signal
            generateOrder(symbol, OrderType::BUY, 100, data.ask);
        } else if (!currentCrossAbove && prevCrossAbove) {
            // Short MA crossed below Long MA - SELL signal
            generateOrder(symbol, OrderType::SELL, 100, data.bid);
        }
        
        prevCrossAbove = currentCrossAbove;
        
        // Show MA values for debugging (every 5th data point to reduce clutter)
        static int counter = 0;
        if (++counter % 5 == 0) {
            std::cout << "  MA Values - Short(" << shortPeriod << "): " 
                      << std::fixed << std::setprecision(3) << shortMA 
                      << ", Long(" << longPeriod << "): " << longMA;
            if (currentCrossAbove) {
                std::cout << " [Short > Long]";
            } else {
                std::cout << " [Short < Long]";
            }
            std::cout << std::endl;
        }
    }
}

void MovingAverageCrossover::onOrderFilled(const Order& order) {
    std::cout << "Order filled: " << order.symbol << std::endl;
}

void MovingAverageCrossover::onTimer() {
    // Timer-based logic if needed
}

// Order default constructor
Order::Order()
    : orderId(0), symbol(""), type(OrderType::BUY), quantity(0), price(0.0) {}