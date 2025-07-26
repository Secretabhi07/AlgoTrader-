#ifndef RISK_MANAGER_H
#define RISK_MANAGER_H

#include <unordered_map>
#include "strategy.h"

class RiskManager {
private:
    double maxPositionSize;
    double maxDailyLoss;
    double currentPnL;
    std::unordered_map<std::string, double> positionLimits;
    
public:
    RiskManager(double maxPos, double maxLoss);
    bool validateOrder(const Order& order, double currentPosition);
    void updatePnL(double pnl);
    void resetDailyPnL();
};

#endif // RISK_MANAGER_H