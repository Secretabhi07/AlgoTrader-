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
        // Generate more sample data to trigger moving average crossover
        std::cout << "Loading sample market data..." << std::endl;
        
        // Start with prices trending down (long MA will be higher)
        for (int i = 0; i < 15; ++i) {
            double price = 152.0 - (i * 0.05); // Prices going down from 152.0 to 151.3
            historicalData.push_back(MarketData("AAPL", price - 0.01, price + 0.01, price, 1000000 + i * 100));
        }
        
        // Then prices start trending up (this will create the crossover signal)
        for (int i = 0; i < 25; ++i) {
            double price = 151.3 + (i * 0.08); // Prices going up from 151.3 to 153.3
            historicalData.push_back(MarketData("AAPL", price - 0.01, price + 0.01, price, 1000000 + (15 + i) * 100));
        }
        
        // Then prices trend down again (to trigger sell signal)
        for (int i = 0; i < 15; ++i) {
            double price = 153.3 - (i * 0.06); // Prices going down
            historicalData.push_back(MarketData("AAPL", price - 0.01, price + 0.01, price, 1000000 + (40 + i) * 100));
        }
        
        std::cout << "Loaded " << historicalData.size() << " data points" << std::endl;
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
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Faster simulation
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
        
        // Process market data - increased count to see the crossover
        MarketData data;
        int dataCount = 0;
        std::cout << "\n=== Starting Market Data Processing ===" << std::endl;
        
        while (dataFeed->getNextData(data) && dataCount < 55) { // Process more data points
            std::cout << "Processing: " << data.symbol 
                      << " Price: $" << std::fixed << std::setprecision(2) << data.last 
                      << " Volume: " << data.volume << std::endl;
            
            strategy->onMarketData(data);
            ++dataCount;
            
            // Small delay to see the output clearly
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        dataFeed->stop();
        std::cout << "\n=== Trading session completed ===" << std::endl;
        std::cout << "Processed " << dataCount << " data points total." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}