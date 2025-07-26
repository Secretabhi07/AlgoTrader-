#include "portfolio.h"

Portfolio::Portfolio(double initialCash) : cash(initialCash), totalPnL(0.0) {}

void Portfolio::updatePosition(const std::string& symbol, int quantity, double price) {
    double currentPos = positions[symbol];
    double currentAvg = avgPrices[symbol];
    
    if (quantity > 0) { // Buy
        if (currentPos >= 0) {
            avgPrices[symbol] = (currentPos * currentAvg + quantity * price) / 
                               (currentPos + quantity);
        }
        positions[symbol] += quantity;
        cash -= quantity * price;
    } else { // Sell
        positions[symbol] += quantity;
        cash -= quantity * price;
        
        double realizedPnL = -quantity * (price - currentAvg);
        totalPnL += realizedPnL;
    }
}

double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& currentPrices) const {
    double unrealizedPnL = 0.0;
    for (const auto& pos : positions) {
        auto priceIt = currentPrices.find(pos.first);
        if (priceIt != currentPrices.end()) {
            unrealizedPnL += pos.second * (priceIt->second - avgPrices.at(pos.first));
        }
    }
    return unrealizedPnL;
}

double Portfolio::getTotalValue(const std::unordered_map<std::string, double>& currentPrices) const {
    double stockValue = 0.0;
    
    // Calculate current market value of all positions
    for (const auto& pos : positions) {
        auto priceIt = currentPrices.find(pos.first);
        if (priceIt != currentPrices.end()) {
            stockValue += pos.second * priceIt->second; // position * current_price
        }
    }
    
    return cash + stockValue;
}