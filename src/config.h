#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <sstream>

class Config {
private:
    std::unordered_map<std::string, std::string> settings;
    
public:
    void loadFromFile(const std::string& filename);
    
    template<typename T>
    T get(const std::string& key, const T& defaultValue) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            std::stringstream ss(it->second);
            T value;
            ss >> value;
            return value;
        }
        return defaultValue;
    }
};

#endif // CONFIG_H