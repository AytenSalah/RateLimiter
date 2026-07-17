#pragma once

#include "Include/Models/IRateLimitStrategy.h"

    // shoofy code omar moheeem
class FixedWindowStrategy : public IRateLimitStrategy {
public:
    CheckResult evaluate(const QuotaState& currentState,
                          const RateLimitConfig& config,
                          QuotaState& updatedState) override {
        auto now = std::chrono::steady_clock::now();

        auto windowStart = currentState.windowStartOrLastRefill;
        auto windowSize   = config.windowOrRefillDuration;
        int  counter      = (int)currentState.usedOrRemaining;
        int  limit        = (int)config.limit;

        
        bool windowOver = (now - windowStart) >= windowSize;


        if (windowOver) 
        {
            counter = 0;
            windowStart = now;
        }

        CheckResult result;
        if (counter < limit) 
        {
            counter = counter + 1;
            result.allowed = true;
        } 
        else 
        {
            result.allowed = false;
        }
       
        // Shoofy code omar 
        result.remaining = limit - counter;
        result.resetOrRetryAfter = windowStart + windowSize;
        updatedState.usedOrRemaining = counter;
        updatedState.windowStartOrLastRefill = windowStart;

        return result;
    }
};
