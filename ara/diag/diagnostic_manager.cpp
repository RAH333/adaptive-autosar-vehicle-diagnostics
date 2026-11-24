#include "diagnostic_manager.hpp"

namespace ara {
namespace diag {

std::vector<uint8_t> DiagnosticManager::HandleUdsRequest(const std::vector<uint8_t>& request, const VehicleHealthData& currentData) {
    if (request.size() >= 3U && request[0] == 0x22U) {
        uint16_t did = (static_cast<uint16_t>(request[1]) << 8U) | static_cast<uint16_t>(request[2]);

        if (did == 0xD101U) {
            return {
                0x62U, 0xD1U, 0x01U,
                static_cast<uint8_t>(currentData.batterySocPercent),
                static_cast<uint8_t>(currentData.motorTempCelsius)
            };
        }
    }

    uint8_t reqSid = request.empty() ? static_cast<uint8_t>(0x00U) : request[0];
    return {0x7FU, reqSid, 0x13U};
}

} // namespace diag
} // namespace ara
