#pragma once

#include "Include/Models/IRateLimitStrategy.h"

class SlidingWindowLogStrategy : public IRateLimitStrategy {
public:
    CheckResult evaluate(const QuotaState& currentState,
                          const RateLimitConfig& config,
                          QuotaState& updatedState) override;
};
