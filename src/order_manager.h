#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <unordered_map>
#include <mutex>
#include <atomic>
#include "strategy.h"

enum class OrderStatus { PENDING, FILLED, CANCELLED, REJECTED };

class OrderManager {
private:
    std::atomic<int> nextOrderId{1};
    std::unordered_map<int, Order> orders;
    std::mutex ordersMutex;
    
    void sendToBroker(const Order& order);
    
public:
    int submitOrder(const std::string& symbol, OrderType type, int quantity, double price);
    void updateOrderStatus(int orderId, OrderStatus status);
    Order getOrder(int orderId);
};

#endif // ORDER_MANAGER_H