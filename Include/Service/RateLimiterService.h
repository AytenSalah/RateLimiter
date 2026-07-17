#pragma once

#include <mutex>
#include <string>

#include "Include/Modelss/IRateLimitStrategy.h"
#include "Include/Models/RateLimitConfig.h"
#include "Include/Store/IQuotaReader.h"
#include "Include/Store/IQuotaWriter.h"


class RateLimiterService {
public:
    RateLimiterService(IQuotaReader& reader,
                        IQuotaWriter& writer,
                        IRateLimitStrategy& strategy,
                        RateLimitConfig config);

    CheckResult checkAndConsume(const std::string& key);

private:
    IQuotaReader& reader_;
    IQuotaWriter& writer_;
    IRateLimitStrategy& strategy_;
    RateLimitConfig config_;

    std::mutex mutex_;
};
