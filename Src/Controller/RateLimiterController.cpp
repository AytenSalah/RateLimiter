#include "Include/Controller/RateLimiterController.h"



RateLimitController::RateLimitController(RateLimiterService& service)
    : service_(service) {}

void RateLimitController::registerRoutes(crow::SimpleApp& app) {

 
    CROW_ROUTE(app, "/check").methods("POST"_method)
    ([this](const crow::request& req) {


        auto body = crow::json::load(req.body);

        if (!body || !body.has("key")) {
            return crow::response(400, "send JSON like: {\"key\": \"client-1\"}");
        }


        std::string key = body["key"].s();

        CheckResult result = service_.checkAndConsume(key);

        crow::json::wvalue responseBody;
        responseBody["allowed"] = result.allowed;
        responseBody["remaining"] = result.remaining;

        return crow::response(200, responseBody);
    });


    CROW_ROUTE(app, "/status/<string>")
    ([this](std::string key) {

    
        CheckResult result = service_.peekStatus(key);

        crow::json::wvalue responseBody;
        responseBody["wouldBeAllowed"] = result.allowed;
        responseBody["remaining"] = result.remaining;

        return crow::response(200, responseBody);
    });


    CROW_ROUTE(app, "/config").methods("POST"_method)
    ([this](const crow::request& req) {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("limit") || !body.has("windowOrRefillMs") || !body.has("strategy")) {
            return crow::response(400, "missing one of: limit, windowOrRefillMs, strategy");
        }

  
        long limit = (long)body["limit"].i();
        long windowMs = (long)body["windowOrRefillMs"].i();
        std::string strategyName = body["strategy"].s();

      
        try {
            
            ParsedConfig parsed = ConfigParser::parse(limit, windowMs, strategyName);

          
            configMutex_.lock();

            service_.updateConfig(parsed.config, parsed.strategy.get());

            
            currentStrategy_ = std::move(parsed.strategy);

            configMutex_.unlock();

            crow::json::wvalue responseBody;
            responseBody["status"] = "config updated";
            return crow::response(200, responseBody);

        } catch (const std::invalid_argument& e) {
            return crow::response(400, e.what());
        }
    });
}
