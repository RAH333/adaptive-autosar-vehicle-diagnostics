#ifndef ARA_COM_SERVICE_REGISTRY_HPP
#define ARA_COM_SERVICE_REGISTRY_HPP

#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

namespace ara {
namespace com {

struct VehicleTelemetryData {
    uint16_t engineRpm;
    int16_t  coolantTempDegC;
    uint16_t batteryVoltageMv;
    bool     overheatWarning;
};

using TelemetryCallback = std::function<void(const VehicleTelemetryData&)>;

/* Simulated SOME/IP Service Discovery & IPC Broker */
class ServiceRegistry {
public:
    static ServiceRegistry& GetInstance();

    void OfferService(const std::string& serviceName);
    bool FindService(const std::string& serviceName) const;

    void PublishEvent(const std::string& serviceName, const VehicleTelemetryData& data);
    void SubscribeEvent(const std::string& serviceName, TelemetryCallback callback);

    void Reset();

private:
    ServiceRegistry() = default;
    mutable std::mutex mtx_;
    std::unordered_map<std::string, bool> offeredServices_;
    std::unordered_map<std::string, TelemetryCallback> subscribers_;
};

} // namespace com
} // namespace ara

#endif // ARA_COM_SERVICE_REGISTRY_HPP
