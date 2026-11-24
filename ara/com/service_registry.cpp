#include "service_registry.hpp"

namespace ara {
namespace com {

ServiceRegistry& ServiceRegistry::GetInstance() {
    static ServiceRegistry instance;
    return instance;
}

void ServiceRegistry::OfferService(const std::string& serviceName) {
    std::lock_guard<std::mutex> lock(mtx_);
    offeredServices_[serviceName] = true;
}

bool ServiceRegistry::FindService(const std::string& serviceName) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = offeredServices_.find(serviceName);
    return (it != offeredServices_.end()) && it->second;
}

void ServiceRegistry::PublishEvent(const std::string& serviceName, const VehicleTelemetryData& data) {
    TelemetryCallback cbCopy = nullptr;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (offeredServices_[serviceName] && subscribers_.count(serviceName)) {
            cbCopy = subscribers_[serviceName];
        }
    }
    if (cbCopy) {
        cbCopy(data);
    }
}

void ServiceRegistry::SubscribeEvent(const std::string& serviceName, TelemetryCallback callback) {
    std::lock_guard<std::mutex> lock(mtx_);
    subscribers_[serviceName] = std::move(callback);
}

void ServiceRegistry::Reset() {
    std::lock_guard<std::mutex> lock(mtx_);
    offeredServices_.clear();
    subscribers_.clear();
}

} // namespace com
} // namespace ara
