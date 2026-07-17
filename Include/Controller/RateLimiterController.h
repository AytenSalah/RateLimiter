#pragma once

#include <memory>
#include <mutex>

#include "crow.h"
#include "Include/Service/RateLimiterService.h"
#include "Include/Strategy/ConfigParser.h"



class RateLimitController {
public:
    RateLimitController(RateLimiterService& service);

    void registerRoutes(crow::SimpleApp& app);

private:
    RateLimiterService& service_;

    std::unique_ptr<IRateLimitStrategy> currentStrategy_;
    std::mutex configMutex_;
};

