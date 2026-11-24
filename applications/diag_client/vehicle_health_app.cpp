#include "../../ara/com/service_registry.hpp"
#include "../../ara/diag/diagnostic_manager.hpp"
#include "../powertrain_provider/powertrain_service.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <rpm> <coolant_temp_C> <battery_mV>\n";
        return 1;
    }

    const uint16_t rpm         = static_cast<uint16_t>(std::atoi(argv[1]));
    const int16_t  coolantTemp = static_cast<int16_t>(std::atoi(argv[2]));
    const uint16_t batteryMv   = static_cast<uint16_t>(std::atoi(argv[3]));

    const std::string kServiceName = "PowertrainTelemetryService_Instance1";

    /* 1. Initialize Adaptive Provider (Skeleton) */
    apps::PowertrainServiceSkeleton provider(kServiceName);
    provider.StartOffering();

    /* 2. Initialize Adaptive Diagnostic Client & Subscribe via ara::com */
    ara::diag::DiagnosticManager diagManager;
    auto& registry = ara::com::ServiceRegistry::GetInstance();

    if (!registry.FindService(kServiceName)) {
        std::cerr << "[ERROR] Service Discovery failed for " << kServiceName << "\n";
        return 2;
    }

    bool receivedWarning = false;
    registry.SubscribeEvent(kServiceName, [&](const ara::com::VehicleTelemetryData& telemetry) {
        receivedWarning = telemetry.overheatWarning;
        diagManager.UpdateVehicleState(telemetry.coolantTempDegC, telemetry.overheatWarning);
    });

    /* 3. Publish Sensor Telemetry over SOME/IP-style Event Bus */
    provider.UpdateAndBroadcastTelemetry(rpm, coolantTemp, batteryMv);

    /* 4. Query UDS Diagnostic Manager (Service 0x22, DID 0xF106: Overheat DTC Status) */
    const std::vector<uint8_t> udsReq  = {0x22U, 0xF1U, 0x06U};
    const std::vector<uint8_t> udsResp = diagManager.HandleUdsRequest(udsReq);

    std::ostringstream hexResp;
    for (size_t i = 0; i < udsResp.size(); ++i) {
        hexResp << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(udsResp[i]);
    }

    std::cout << "ADAPTIVE_SIL|SD_FOUND:1|OVERHEAT_WARN:" << (receivedWarning ? 1 : 0)
              << "|UDS_HEX:" << hexResp.str() << "\n";

    return 0;
}
