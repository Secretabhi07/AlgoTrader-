🚀 Algorithmic Trading System
# 🚀 Algorithmic Trading System



A high-performance algorithmic trading system built in C++ featuring real-time market data processing, strategy implementation, risk management, and portfolio tracking.

## 🌟 Features

- **📊 Real-time Market Data Processing**: Thread-safe data feed with queue-based architecture
- **🧠 Trading Strategies**: Modular strategy framework with Moving Average Crossover implementation
- **⚡ Order Management**: Asynchronous order execution and tracking system
- **🛡️ Risk Management**: Position limits, daily loss limits, and comprehensive risk controls
- **💼 Portfolio Management**: Real-time P&L tracking, position management, and performance analytics
- **⚙️ Configuration Management**: File-based configuration system for easy parameter tuning
- **🧪 Comprehensive Testing**: Full test suite with 34+ unit tests
- **🔧 Modern C++**: Built with C++17 features, thread-safe design, and RAII principles

## 🏗️ Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Market Data   │───▶│    Strategy     │───▶│ Order Manager   │
│      Feed       │    │   (MA Cross)    │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Data Queue    │    │ Risk Manager    │    │   Portfolio     │
│  (Thread-Safe)  │    │                 │    │   Tracker       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## 🔧 System Requirements

### Minimum Requirements
- **OS**: macOS 10.15+, Ubuntu 18.04+, Windows 10+ (with WSL2)
- **Compiler**: GCC 8.0+, Clang 10.0+, or MSVC 2019+
- **CMake**: Version 3.15 or higher
- **RAM**: 512 MB minimum
- **CPU**: Any modern x64 processor

### Recommended Requirements
- **OS**: macOS 12+, Ubuntu 20.04+
- **Compiler**: GCC 11.0+, Clang 12.0+
- **CMake**: Version 3.20+
- **RAM**: 2 GB for optimal performance
- **CPU**: Multi-core processor for concurrent data processing

## 📦 Installation

### Clone the Repository
```bash
git clone https://github.com/your-username/AlgoTradingSystem.git
cd AlgoTradingSystem
```

### Install Dependencies

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake

# Or using MacPorts
sudo port install cmake
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libpthread-stubs0-dev
```

#### CentOS/RHEL
```bash
sudo yum groupinstall "Development Tools"
sudo yum install cmake3
```

## 🚀 Quick Start

### 1. Build the Project
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project (use -j4 for 4 parallel jobs)
make -j4
```

### 2. Run the Main Application
```bash
# From the build directory
./AlgoTradingSystem
```

### 3. Run Tests
```bash
# Run all tests
./RunTests

# Or use CTest for detailed output
ctest --verbose

# Or use the custom target
make test_all
```

## 📁 Project Structure

```
AlgoTradingSystem/
├── 📄 CMakeLists.txt          # Build configuration
├── 📄 README.md               # This file
├── 📄 requirements.txt        # System requirements
├── 📄 config.txt              # Runtime configuration
├── 📁 src/                    # Source code
│   ├── 🧠 main.cpp            # Main application entry point
│   ├── 📊 market_data.h/cpp   # Market data structures and feed
│   ├── 🎯 strategy.h/cpp      # Trading strategy implementations
│   ├── 📝 order_manager.h/cpp # Order execution and management
│   ├── 🛡️ risk_manager.h/cpp  # Risk management and controls
│   ├── 💼 portfolio.h/cpp     # Portfolio and P&L tracking
│   └── ⚙️ config.h/cpp        # Configuration management
├── 📁 tests/                  # Test suite
│   └── 🧪 test_strategy.cpp   # Comprehensive unit tests
├── 📁 data/                   # Sample data files
│   └── 📈 sample_data.csv     # Historical market data
└── 📁 build/                  # Build artifacts (created during build)
```

## ⚙️ Configuration

The system uses a configuration file (`config.txt`) for runtime parameters:

```ini
# Risk Management Settings
max_position_size=10000.0
daily_loss_limit=5000.0
max_drawdown=0.05

# Strategy Parameters
short_ma_period=5
long_ma_period=20
initial_cash=100000.0

# Data Feed Settings
data_update_interval=100
simulation_speed=50

# Logging
debug_mode=true
log_trades=true
```

## 🧠 Trading Strategies

### Moving Average Crossover
The primary strategy implemented uses a **Moving Average Crossover** algorithm:

- **Short MA (5 periods)**: Recent price trend
- **Long MA (20 periods)**: Longer-term price trend
- **Buy Signal**: When Short MA crosses above Long MA
- **Sell Signal**: When Short MA crosses below Long MA

```cpp
// Example strategy configuration
MovingAverageCrossover strategy("AAPL", 5, 20, 100000.0);
```


## 🧪 Testing

The project includes a comprehensive test suite covering:

### Test Categories
- ✅ **Order Class Tests**: Constructor validation and property checks
- ✅ **Market Data Tests**: Data structure integrity and thread safety
- ✅ **Portfolio Tests**: P&L calculations, position tracking, total value
- ✅ **Risk Manager Tests**: Position limits, loss limits, order validation
- ✅ **Order Manager Tests**: Order submission, tracking, and retrieval
- ✅ **Strategy Tests**: Moving average calculations and signal generation

### Running Specific Tests
```bash
# Run with verbose output
./RunTests

## 🔧 Development

### Adding New Strategies
1. Inherit from the `Strategy` base class
2. Implement the required virtual methods:
   ```cpp
   class MyStrategy : public Strategy {
   public:
       void onMarketData(const MarketData& data) override;
       void onOrderFilled(const Order& order) override;
       void onTimer() override;
   };
   ```

### Adding New Tests
1. Add test functions to `tests/test_strategy.cpp`
2. Use the `TestFramework` class for assertions
3. Rebuild and run tests

### Performance Optimization
- Use `-DCMAKE_BUILD_TYPE=Release` for production builds
- Enable compiler optimizations: `-O3 -march=native`
- Profile with tools like `perf` or `Instruments` (macOS)

## 📈 Performance Metrics

- **Data Processing**: ~100,000 ticks/second
- **Order Latency**: <1ms average execution time
- **Memory Usage**: <50MB typical runtime
- **CPU Usage**: <10% on modern processors



### Code Style
- Follow C++17 standards
- Use meaningful variable names
- Add comments for complex algorithms
- Include unit tests for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Modern C++ Design**: Inspired by Scott Meyers' Effective C++
- **Financial Algorithms**: Based on quantitative finance principles
- **Architecture**: Event-driven design patterns
- **Testing**: Google Test framework concepts

## 📞 Support

- 📧 **Email**: einsteinabhijit@gmail.com
- 🐛 **Issues**: [GitHub Issues](https://github.com/Secretabhi07/AlgoTradingSystem/issues)


## 🔮 Future Enhancements

- [ ] **Web Dashboard**: Real-time monitoring interface
- [ ] **Machine Learning**: AI-powered strategy optimization
- [ ] **Multi-Asset Support**: Forex, crypto, commodities
- [ ] **Backtesting Engine**: Historical strategy validation
- [ ] **REST API**: External integration capabilities
- [ ] **Database Integration**: Persistent data storage
- [ ] **Real Broker Integration**: Live trading capabilities

---

⭐ **Star this repository if you found it helpful!**

**Built with ❤️ and modern C++**
