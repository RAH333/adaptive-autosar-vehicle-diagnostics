#include "powertrain_service.hpp"
#include <utility>

namespace apps {

PowertrainServiceSkeleton::PowertrainServiceSkeleton(std::string instanceName)
    : instanceName_(std::move(instanceName)) {}

void PowertrainServiceSkeleton::StartOffering() {
    ara::com::ServiceRegistry::GetInstance().OfferService(instanceName_);
}

void PowertrainServiceSkeleton::UpdateAndBroadcastTelemetry(uint16_t rpm, int16_t coolantTemp, uint16_t batteryMv) {
    ara::com::VehicleTelemetryData data{};
    data.engineRpm        = rpm;
    data.coolantTempDegC  = coolantTemp;
    data.batteryVoltageMv = batteryMv;
    data.overheatWarning  = (coolantTemp > kOverheatThresholdDegC);

    ara::com::ServiceRegistry::GetInstance().PublishEvent(instanceName_, data);
}

} // namespace apps
