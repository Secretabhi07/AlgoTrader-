#include <iostream>
#include <cassert>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../src/strategy.h"
#include "../src/market_data.h"
#include "../src/portfolio.h"
#include "../src/risk_manager.h"
#include "../src/order_manager.h"

// Simple test framework
class TestFramework {
private:
    int totalTests = 0;
    int passedTests = 0;
    
public:
    void assert_equal(double expected, double actual, double tolerance = 0.001, const std::string& testName = "") {
        totalTests++;
        if (std::abs(expected - actual) <= tolerance) {
            std::cout << "✅ PASS: " << testName << " (Expected: " << expected << ", Got: " << actual << ")" << std::endl;
            passedTests++;
        } else {
            std::cout << "❌ FAIL: " << testName << " (Expected: " << expected << ", Got: " << actual << ")" << std::endl;
        }
    }
    
    void assert_true(bool condition, const std::string& testName = "") {
        totalTests++;
        if (condition) {
            std::cout << "✅ PASS: " << testName << std::endl;
            passedTests++;
        } else {
            std::cout << "❌ FAIL: " << testName << std::endl;
        }
    }
    
    void printSummary() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TEST SUMMARY: " << passedTests << "/" << totalTests << " tests passed" << std::endl;
        if (passedTests == totalTests) {
            std::cout << "🎉 ALL TESTS PASSED!" << std::endl;
        } else {
            std::cout << "⚠️  " << (totalTests - passedTests) << " tests failed" << std::endl;
        }
        std::cout << std::string(50, '=') << std::endl;
    }
};

// Mock strategy class to capture generated orders
class TestableMovingAverageCrossover : public MovingAverageCrossover {
public:
    std::vector<Order> generatedOrders;
    
    TestableMovingAverageCrossover(const std::string& sym, int shortP, int longP, double initialCash)
        : MovingAverageCrossover(sym, shortP, longP, initialCash) {}
    
protected:
    void generateOrder(const std::string& symbol, OrderType type, int quantity, double price) override {
        // Capture the order instead of just printing
        static int orderId = 1;
        Order order(orderId++, symbol, type, quantity, price);
        generatedOrders.push_back(order);
        
        std::cout << "📋 Captured Order: " << symbol 
                  << (type == OrderType::BUY ? " BUY " : " SELL ")
                  << quantity << " @ $" << std::fixed << std::setprecision(2) << price << std::endl;
    }
};

// Test data generator
std::vector<MarketData> generateTestData() {
    std::vector<MarketData> data;
    
    // Phase 1: Declining prices (Long MA will be higher than Short MA)
    std::cout << "📊 Generating Phase 1: Declining prices..." << std::endl;
    for (int i = 0; i < 15; i++) {
        double price = 100.0 - (i * 0.5); // 100.0 down to 93.0
        data.emplace_back("AAPL", price - 0.05, price + 0.05, price, 1000000 + i * 1000);
    }
    
    // Phase 2: Rising prices (This should trigger BUY signal)
    std::cout << "📊 Generating Phase 2: Rising prices..." << std::endl;
    for (int i = 0; i < 20; i++) {
        double price = 93.0 + (i * 0.8); // 93.0 up to 108.2
        data.emplace_back("AAPL", price - 0.05, price + 0.05, price, 1000000 + (15 + i) * 1000);
    }
    
    // Phase 3: Declining prices again (This should trigger SELL signal)
    std::cout << "📊 Generating Phase 3: Declining prices again..." << std::endl;
    for (int i = 0; i < 15; i++) {
        double price = 108.2 - (i * 0.6); // 108.2 down to 99.8
        data.emplace_back("AAPL", price - 0.05, price + 0.05, price, 1000000 + (35 + i) * 1000);
    }
    
    return data;
}

void testMovingAverageCrossover(TestFramework& tf) {
    std::cout << "\n🧪 Testing Moving Average Crossover Strategy..." << std::endl;
    
    TestableMovingAverageCrossover strategy("AAPL", 5, 10, 100000.0); // Shorter periods for faster testing
    std::vector<MarketData> testData = generateTestData();
    
    // Feed data to strategy
    for (const auto& data : testData) {
        strategy.onMarketData(data);
    }
    
    // Verify we got some orders
    tf.assert_true(strategy.generatedOrders.size() >= 2, "Should generate at least 2 orders (1 BUY, 1 SELL)");
    
    if (strategy.generatedOrders.size() >= 2) {
        // First significant order should be a BUY (when short MA crosses above long MA)
        bool foundBuy = false, foundSell = false;
        for (const auto& order : strategy.generatedOrders) {
            if (order.type == OrderType::BUY && !foundBuy) {
                foundBuy = true;
                tf.assert_true(order.quantity == 100, "Buy order should be for 100 shares");
                tf.assert_true(order.symbol == "AAPL", "Buy order should be for AAPL");
            }
            if (order.type == OrderType::SELL && foundBuy && !foundSell) {
                foundSell = true;
                tf.assert_true(order.quantity == 100, "Sell order should be for 100 shares");
                tf.assert_true(order.symbol == "AAPL", "Sell order should be for AAPL");
            }
        }
        
        tf.assert_true(foundBuy, "Should generate a BUY order when short MA crosses above long MA");
        tf.assert_true(foundSell, "Should generate a SELL order when short MA crosses below long MA");
    }
    
    std::cout << "📈 Generated " << strategy.generatedOrders.size() << " orders total" << std::endl;
}

void testMarketData(TestFramework& tf) {
    std::cout << "\n🧪 Testing MarketData class..." << std::endl;
    
    MarketData data1("AAPL", 150.0, 150.5, 150.25, 1000000);
    tf.assert_equal(150.0, data1.bid, 0.001, "MarketData bid price");
    tf.assert_equal(150.5, data1.ask, 0.001, "MarketData ask price");
    tf.assert_equal(150.25, data1.last, 0.001, "MarketData last price");
    tf.assert_true(data1.symbol == "AAPL", "MarketData symbol");
    tf.assert_true(data1.volume == 1000000, "MarketData volume");
    
    MarketData data2; // Default constructor
    tf.assert_true(data2.symbol.empty(), "Default MarketData should have empty symbol");
    tf.assert_equal(0.0, data2.bid, 0.001, "Default MarketData bid should be 0");
}

void testPortfolio(TestFramework& tf) {
    std::cout << "\n🧪 Testing Portfolio class..." << std::endl;
    
    Portfolio portfolio(100000.0);
    tf.assert_equal(100000.0, portfolio.getCash(), 0.001, "Initial cash amount");
    
    // Test buying stocks
    portfolio.updatePosition("AAPL", 100, 150.0); // Buy 100 shares at $150
    tf.assert_equal(85000.0, portfolio.getCash(), 0.001, "Cash after buying 100 shares at $150");
    
    // Test current prices for P&L calculation
    std::unordered_map<std::string, double> currentPrices = {{"AAPL", 155.0}};
    
    double unrealizedPnL = portfolio.getUnrealizedPnL(currentPrices);
    tf.assert_equal(500.0, unrealizedPnL, 0.001, "Unrealized P&L should be $500 (100 * (155 - 150))");
    
    double totalValue = portfolio.getTotalValue(currentPrices);
    tf.assert_equal(100500.0, totalValue, 0.001, "Total portfolio value should be $100,500");
}

void testRiskManager(TestFramework& tf) {
    std::cout << "\n🧪 Testing RiskManager class..." << std::endl;
    
    RiskManager riskManager(10000.0, 5000.0); // Max position $10k, max daily loss $5k
    
    // Test valid order
    Order validOrder(1, "AAPL", OrderType::BUY, 50, 150.0); // $7,500 position
    tf.assert_true(riskManager.validateOrder(validOrder, 0.0), "Valid order should be approved");
    
    // Test order that exceeds position limit
    Order invalidOrder(2, "AAPL", OrderType::BUY, 100, 150.0); // $15,000 position
    tf.assert_true(!riskManager.validateOrder(invalidOrder, 0.0), "Order exceeding position limit should be rejected");
    
    // Test daily loss limit
    riskManager.updatePnL(-6000.0); // Set loss beyond limit
    tf.assert_true(!riskManager.validateOrder(validOrder, 0.0), "Order should be rejected when daily loss limit exceeded");
}

void testOrderManager(TestFramework& tf) {
    std::cout << "\n🧪 Testing OrderManager class..." << std::endl;
    
    OrderManager orderManager;
    
    // Submit an order
    int orderId = orderManager.submitOrder("AAPL", OrderType::BUY, 100, 150.0);
    tf.assert_true(orderId > 0, "Order ID should be positive");
    
    // Retrieve the order
    Order retrievedOrder = orderManager.getOrder(orderId);
    tf.assert_true(retrievedOrder.symbol == "AAPL", "Retrieved order should have correct symbol");
    tf.assert_true(retrievedOrder.type == OrderType::BUY, "Retrieved order should have correct type");
    tf.assert_equal(100, retrievedOrder.quantity, 0.001, "Retrieved order should have correct quantity");
    tf.assert_equal(150.0, retrievedOrder.price, 0.001, "Retrieved order should have correct price");
}

void testOrderClass(TestFramework& tf) {
    std::cout << "\n🧪 Testing Order class..." << std::endl;
    
    // Test parameterized constructor
    Order order1(1, "AAPL", OrderType::BUY, 100, 150.0);
    tf.assert_true(order1.orderId == 1, "Order ID should be 1");
    tf.assert_true(order1.symbol == "AAPL", "Order symbol should be AAPL");
    tf.assert_true(order1.type == OrderType::BUY, "Order type should be BUY");
    tf.assert_equal(100, order1.quantity, 0.001, "Order quantity should be 100");
    tf.assert_equal(150.0, order1.price, 0.001, "Order price should be 150.0");
    
    // Test default constructor
    Order order2;
    tf.assert_true(order2.orderId == 0, "Default order ID should be 0");
    tf.assert_true(order2.symbol.empty(), "Default order symbol should be empty");
    tf.assert_true(order2.type == OrderType::BUY, "Default order type should be BUY");
}

int main() {
    std::cout << "🚀 Starting Algorithmic Trading System Tests..." << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    TestFramework tf;
    
    try {
        testOrderClass(tf);
        testMarketData(tf);
        testPortfolio(tf);
        testRiskManager(tf);
        testOrderManager(tf);
        testMovingAverageCrossover(tf);
        
    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    tf.printSummary();
    
    std::cout << "\n🎯 Test completed successfully!" << std::endl;
    return 0;
}