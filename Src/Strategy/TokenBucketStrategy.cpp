#include "Src/Strategytrategy/TokenBucketStrategy.h"


CheckResult TokenBucketStrategy::evaluate(const QuotaState& currentState,
                                           const RateLimitConfig& config,
                                           QuotaState& updatedState) {
    auto now = std::chrono::steady_clock::now();

    auto lastRefill = currentState.windowStartOrLastRefill;
    double tokens    = currentState.usedOrRemaining;
    auto refillTime  = config.windowOrRefillDuration; // time to refill `limit` tokens
    int  limit       = (int)config.limit;

    // 1. how much time passed since we last refilled?
    long msPassed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRefill).count();
    long msRefill = std::chrono::duration_cast<std::chrono::milliseconds>(refillTime).count();

    // 2. how many tokens does that time earn us?
    double tokensEarned = (double)msPassed / (double)msRefill * limit;

    // 3. add earned tokens, but never go above the bucket size (limit)
    tokens = tokens + tokensEarned;
    if (tokens > limit) {
        tokens = limit;
    }

    // 4. allow or reject
    CheckResult result;
    if (tokens >= 1.0) {
        tokens = tokens - 1.0;
        result.allowed = true;
    } else {
        result.allowed = false;
    }

    // 5. save everything back - "now" becomes the new last-refill point
    result.remaining = tokens;
    updatedState.usedOrRemaining = tokens;
    updatedState.windowStartOrLastRefill = now;

    // 6. when will we have at least 1 token again? (only matters if denied)
    double tokensNeeded = 1.0 - tokens;
    double msUntilNextToken = tokensNeeded / limit * msRefill;
    result.resetOrRetryAfter = now + std::chrono::milliseconds((long)msUntilNextToken);

    return result;
}
