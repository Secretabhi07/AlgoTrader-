#include "order_manager.h"
#include <iostream>

int OrderManager::submitOrder(const std::string& symbol, OrderType type, int quantity, double price) {
    int orderId = nextOrderId++;
    Order order(orderId, symbol, type, quantity, price);
    
    {
        std::lock_guard<std::mutex> lock(ordersMutex);
        orders[orderId] = order;
    }
    
    sendToBroker(order);
    return orderId;
}

void OrderManager::updateOrderStatus(int orderId, OrderStatus status) {
    std::lock_guard<std::mutex> lock(ordersMutex);
    auto it = orders.find(orderId);
    if (it != orders.end()) {
        std::cout << "Order " << orderId << " status updated" << std::endl;
    }
}

Order OrderManager::getOrder(int orderId) {
    std::lock_guard<std::mutex> lock(ordersMutex);
    return orders.at(orderId);
}

void OrderManager::sendToBroker(const Order& order) {
    std::cout << "Sending order to broker: " << order.symbol << std::endl;
}