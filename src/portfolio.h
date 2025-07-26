#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <unordered_map>
#include <string>

class Portfolio {
private:
    std::unordered_map<std::string, double> positions;
    std::unordered_map<std::string, double> avgPrices;
    double cash;
    double totalPnL;
    
public:
    Portfolio(double initialCash);
    void updatePosition(const std::string& symbol, int quantity, double price);
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& currentPrices) const;
    double getTotalValue(const std::unordered_map<std::string, double>& currentPrices) const;
    double getCash() const { return cash; }
};

#endif // PORTFOLIO_H