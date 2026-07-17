#pragma once
#include "Include/Models/IRateLimitStrategy.h"

class TokenBucketStrategy :public IRateLimitStrategy
{
public:
 checkResult evaluate(const QuotaState& currentState,
                          const RateLimitConfig& config,
                          QuotaState& updatedState) override;
};
