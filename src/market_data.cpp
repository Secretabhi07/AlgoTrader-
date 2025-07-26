#include "market_data.h"

// MarketData default constructor
MarketData::MarketData()
    : symbol(""), bid(0.0), ask(0.0), last(0.0), volume(0), 
      timestamp(std::chrono::high_resolution_clock::now()) {}

// MarketData parameterized constructor
MarketData::MarketData(const std::string& sym, double b, double a, double l, int64_t v)
    : symbol(sym), bid(b), ask(a), last(l), volume(v), 
      timestamp(std::chrono::high_resolution_clock::now()) {}

// DataFeed implementation
void DataFeed::addData(const MarketData& data) {
    std::lock_guard<std::mutex> lock(queueMutex);
    dataQueue.push(data);
    cv.notify_one();
}

bool DataFeed::getNextData(MarketData& data) {
    std::unique_lock<std::mutex> lock(queueMutex);
    cv.wait_for(lock, std::chrono::milliseconds(100), [this] { return !dataQueue.empty() || !running; });
    
    if (!dataQueue.empty()) {
        data = dataQueue.front();
        dataQueue.pop();
        return true;
    }
    return false;
}