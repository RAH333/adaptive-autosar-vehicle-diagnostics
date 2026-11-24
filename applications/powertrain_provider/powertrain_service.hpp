#ifndef POWERTRAIN_SERVICE_HPP
#define POWERTRAIN_SERVICE_HPP

#include "../../ara/com/service_registry.hpp"
#include <string>

namespace apps {

class PowertrainServiceSkeleton {
public:
    explicit PowertrainServiceSkeleton(std::string instanceName);

    void StartOffering();
    void UpdateAndBroadcastTelemetry(uint16_t rpm, int16_t coolantTemp, uint16_t batteryMv);

private:
    static constexpr int16_t kOverheatThresholdDegC = 105;
    std::string instanceName_;
};

} // namespace apps

#endif // POWERTRAIN_SERVICE_HPP
