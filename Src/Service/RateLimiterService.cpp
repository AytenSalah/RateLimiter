#include "Include/Service/RateLimiterService.h"

RateLimiterService::RateLimiterService(IQuotaReader& reader,
                                        IQuotaWriter& writer,
                                        IRateLimitStrategy& strategy,
                                        RateLimitConfig config)
    : reader_(reader), writer_(writer), strategy_(strategy), config_(config) {}

CheckResult RateLimiterService::checkAndConsume(const std::string& key) {
    
    std::lock_guard<std::mutex> lock(mutex_);
    QuotaState currentState;
    auto existing = reader_.get(key);
    if (existing.has_value()) 
    {
        currentState = existing.value();
    } 
    else 
    {
        currentState.windowStartOrLastRefill = std::chrono::steady_clock::now();
    }

    QuotaState updatedState;
    CheckResult result = strategy_.evaluate(currentState, config_, updatedState);


    if (result.allowed) 
    {
        writer_.set(key, updatedState);
    }

    return result;
}
