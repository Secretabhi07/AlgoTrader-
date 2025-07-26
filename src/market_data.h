#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <string>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>

class MarketData {
public:
    std::string symbol;
    double bid, ask, last;
    int64_t volume;
    std::chrono::high_resolution_clock::time_point timestamp;
    MarketData();
    MarketData(const std::string& sym, double b, double a, double l, int64_t v);
};

class DataFeed {
protected:
    std::queue<MarketData> dataQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool running = false;
    
public:
    virtual ~DataFeed() = default;
    virtual void subscribe(const std::string& symbol) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    
    void addData(const MarketData& data);
    bool getNextData(MarketData& data);
};

#endif // MARKET_DATA_H