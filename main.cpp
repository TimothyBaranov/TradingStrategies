#include <iostream>
#include <vector>
#include <map>
#include <cmath> // Для std::sqrt
#include <numeric> // Для std::accumulate

// Псевдоданные для сделок и стакана
struct Trade {
    double price;
    double volume;
};

struct MarketDepth {
    double bestBid;
    double bestAsk;
    std::vector<std::pair<double, double>> bidLevels; // цена -> объем
    std::vector<std::pair<double, double>> askLevels; // цена -> объем

    MarketDepth() {
        bestBid = 100.0; // Лучшая цена покупки
        bestAsk = 101.0; // Лучшая цена продажи

        bidLevels.emplace_back(99.5, 10);
        bidLevels.emplace_back(99.0, 20);
        bidLevels.emplace_back(98.5, 15);

        askLevels.emplace_back(101.5, 5);
        askLevels.emplace_back(102.0, 25);
        askLevels.emplace_back(102.5, 10);
    }
};

class Instrument {
public:
    MarketDepth marketDepth() const {
        return MarketDepth(); // Возвращаем псевдоданные стакана
    }

    std::vector<Trade> trades() const {
        return {
            {100.5, 1}, {100.7, 2}, {100.8, 1.5}, {101.2, 3} // Псевдоданные сделок
        };
    }
};

class MyStrategy {
public:
    bool onOrders(const Instrument & instrument);
    bool onTrades(const Instrument & instrument);
};

// Реализация первой стратегии: расчет дисбаланса в стакане
bool MyStrategy::onOrders(const Instrument & instrument) {
    MarketDepth depth = instrument.marketDepth();
    
    double totalBidVolume = 0; 
    double totalAskVolume = 0;

    // Суммируем объемы на уровнях бидов и асков в одном цикле
    for (const auto & level : depth.bidLevels) {
        totalBidVolume += level.second; 
    }
    
    for (const auto & level : depth.askLevels) {
        totalAskVolume += level.second;
    }

    // Расчет дисбаланса
    if (totalBidVolume > totalAskVolume) {
        std::cout << "Placing order on the bid side." << std::endl;
        // Код для выставления заявки...
    } else {
        std::cout << "Market order on the ask side." << std::endl;
        // Код для удара по рынку...
    }

    return true; 
}

// Реализация второй стратегии: расчет волатильности и ликвидности
bool MyStrategy::onTrades(const Instrument & instrument) {
    auto trades = instrument.trades(); 
    
    if (trades.empty()) {
        std::cerr << "No trades available." << std::endl;
        return false; // Обработка случая отсутствия сделок
    }

    double totalPrice = 0; 
    double totalVolume = 0;

    for (const auto & trade : trades) {
        totalPrice += trade.price; 
        totalVolume += trade.volume; 
    }

    double averagePrice = totalPrice / trades.size(); 

   // Расчет волатильности (стандартное отклонение)
   double variance = 0;

   for (const auto & trade : trades) {
       variance += std::pow(trade.price - averagePrice, 2);
   }
   
   variance /= trades.size();
   
   double volatility = std::sqrt(variance); 

   MarketDepth depth = instrument.marketDepth();
   
   double liquidity = 0; 

   for (const auto & level : depth.askLevels) {
       liquidity += level.second; 
   }

   if (volatility > 1.0) { 
       std::cout << "Quoting on the liquid side." << std::endl;
       // Код для выставления заявки...
   } else {
       std::cout << "Entering less liquid side." << std::endl;
       // Код для выставления заявки...
   }

   return true; 
}

int main() {
   MyStrategy strategy;
   
   Instrument instrument;

   strategy.onOrders(instrument);
   strategy.onTrades(instrument);

   return 0;
}
