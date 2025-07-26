#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "market_data.h"
#include "strategy.h"
#include "order_manager.h"
#include "risk_manager.h"
#include "portfolio.h"
#include "config.h"

// Simple CSV Data Feed for demonstration
class CSVDataFeed : public DataFeed {
private:
    std::vector<MarketData> historicalData;
    size_t currentIndex = 0;
    std::thread feedThread;
    
public:
    void loadData() {
        // Sample data - in real implementation, load from CSV file
        historicalData.push_back(MarketData("AAPL", 150.0, 150.1, 150.05, 1000000));
        historicalData.push_back(MarketData("AAPL", 150.1, 150.2, 150.15, 1000500));
        historicalData.push_back(MarketData("AAPL", 150.2, 150.3, 150.25, 1001000));
        // Add more sample data...
        for (int i = 0; i < 100; ++i) {
            double price = 150.0 + (i * 0.1);
            historicalData.push_back(MarketData("AAPL", price, price + 0.1, price + 0.05, 1000000 + i * 100));
        }
    }
    
    void subscribe(const std::string& symbol) override {
        std::cout << "Subscribed to: " << symbol << std::endl;
    }
    
    void start() override {
        running = true;
        feedThread = std::thread([this]() {
            for (const auto& data : historicalData) {
                if (!running) break;
                
                addData(data);
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate real-time
            }
        });
    }
    
    void stop() override {
        running = false;
        if (feedThread.joinable()) {
            feedThread.join();
        }
    }
};

int main() {
    try {
        std::cout << "Starting Algorithmic Trading System..." << std::endl;
        
        // Load configuration
        Config config;
        config.loadFromFile("config.txt");
        
        // Initialize components
        auto dataFeed = std::make_unique<CSVDataFeed>();
        auto strategy = std::make_unique<MovingAverageCrossover>("AAPL", 5, 20, 100000.0);
        auto orderManager = std::make_unique<OrderManager>();
        auto riskManager = std::make_unique<RiskManager>(10000.0, 5000.0);
        Portfolio portfolio(100000.0);
        
        // Load sample data
        dataFeed->loadData();
        dataFeed->subscribe("AAPL");
        dataFeed->start();
        
        // Process market data
        MarketData data;
        int dataCount = 0;
        while (dataFeed->getNextData(data) && dataCount < 50) {
            std::cout << "Processing: " << data.symbol 
                      << " Price: $" << data.last 
                      << " Volume: " << data.volume << std::endl;
            
            strategy->onMarketData(data);
            ++dataCount;
        }
        
        dataFeed->stop();
        std::cout << "Trading session completed." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}