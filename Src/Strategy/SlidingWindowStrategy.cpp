#pragma once

#include "Include/Models/IRateLimitStrategy.hpp"

class SlidingWindowLogStrategy : public IRateLimitStrategy {
public:
    CheckResult evaluate(const QuotaState& currentState,
                          const RateLimitConfig& config,
                          QuotaState& updatedState) override 
        {
        auto now = std::chrono::steady_clock::now();
        auto windowSize = config.windowOrRefillDuration;
        int limit = (int)config.limit;
        queue timestamps = currentState.requestTimestamps;
        auto miniTimeAllowed = now - windowSize;

        while (!timestamps.empty() && timestamps.front() < miniTimeAllowed) 
        {
            timestamps.pop_front();
        }

        int countInWindow = (int)timestamps.size();

        CheckResult result;
        if (countInWindow < limit) {
            timestamps.push_back(now);
            countInWindow = countInWindow + 1;
            result.allowed = true;
        } 
        else 
        {
            result.allowed = false;
        }

        
        if (!timestamps.empty()) {
            result.resetOrRetryAfter = timestamps.front() + windowSize; //need omar variabels 
        } 
        else 
        {
            result.resetOrRetryAfter = now; //need omar variabels
        }

        result.remaining = limit - countInWindow;
        updatedState.requestTimestamps = timestamps;

        return result;
    }
};
