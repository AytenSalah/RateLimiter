#pragma once

#include <memory>
#include <string>

#include "Src/Models/IRateLimitStrategy.h"
#include "Src/Models/RateLimitConfig.h"


struct ParsedConfig {
    RateLimitConfig config;
    std::unique_ptr<IRateLimitStrategy> strategy;
};

/
class ConfigParser {
public:
    static ParsedConfig parse(long limit, long windowOrRefillMs, const std::string& strategyName);
};