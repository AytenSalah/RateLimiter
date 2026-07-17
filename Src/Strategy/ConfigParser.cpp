#include "Src/Strategy/ConfigParser.h"
#include <stdexcept>

#include "Src/Strategy/FixedWindowStrategy.h"
#include "Src/Strategy/SlidingWindowLogStrategy.h"
#include "Src/Strategy/TokenBucketStrategy.h"


ParsedConfig ConfigParser::parse(long limit, long windowOrRefillMs, const std::string& strategyName) {
  
    if (limit <= 0) 
    {
        throw std::invalid_argument("limit must be a positive number");
    }
    if (windowOrRefillMs <= 0) 
    { 
        throw std::invalid_argument("window Or Refill Duration must be a positive number");
    }


    RateLimitConfig config;
    config.limit = limit;
    config.windowOrRefillDuration = std::chrono::milliseconds(windowOrRefillMs); //omar varibales 

    ParsedConfig result;


    if (strategyName == "fixed_window") 
    {
        config.strategy = StrategyType::FixedWindow;
        result.strategy = std::make_unique<FixedWindowStrategy>();
    } 
    else if (strategyName == "sliding_window") 
    {
        config.strategy = StrategyType::SlidingWindow;
        result.strategy = std::make_unique<SlidingWindowLogStrategy>();
    } 
    else if (strategyName == "token_bucket") 
    {
        config.strategy = StrategyType::TokenBucket;
        result.strategy = std::make_unique<TokenBucketStrategy>();
    }
     else 
    {
        throw std::invalid_argument("unknown strategy name: " + strategyName);
    }

    result.config = config;
    return result;
}
