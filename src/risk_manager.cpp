
#include "risk_manager.h"
#include <iostream>
#include <cmath>

RiskManager::RiskManager(double maxPos, double maxLoss) 
    : maxPositionSize(maxPos), maxDailyLoss(maxLoss), currentPnL(0.0) {}

bool RiskManager::validateOrder(const Order& order, double currentPosition) {
    // Check position size limits
    double newPosition = currentPosition;
    if (order.type == OrderType::BUY) {
        newPosition += order.quantity;
    } else {
        newPosition -= order.quantity;
    }
    
    if (std::abs(newPosition) > maxPositionSize) {
        std::cout << "Order rejected: Position size limit exceeded" << std::endl;
        return false;
    }
    
    // Check daily loss limit
    if (currentPnL < -maxDailyLoss) {
        std::cout << "Order rejected: Daily loss limit exceeded" << std::endl;
        return false;
    }
    
    // Check symbol-specific limits if they exist
    auto it = positionLimits.find(order.symbol);
    if (it != positionLimits.end() && std::abs(newPosition) > it->second) {
        std::cout << "Order rejected: Symbol position limit exceeded" << std::endl;
        return false;
    }
    
    return true;
}

void RiskManager::updatePnL(double pnl) {
    currentPnL += pnl;
    std::cout << "Updated P&L: $" << currentPnL << std::endl;
}

void RiskManager::resetDailyPnL() {
    currentPnL = 0.0;
    std::cout << "Daily P&L reset" << std::endl;
}