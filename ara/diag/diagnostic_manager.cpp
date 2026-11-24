#include "diagnostic_manager.hpp"

namespace ara {
namespace diag {

DiagnosticManager::DiagnosticManager()
    : latestCoolantTemp_(25), overheatDtcActive_(false) {}

void DiagnosticManager::UpdateVehicleState(int16_t coolantTemp, bool overheatDtcActive) {
    latestCoolantTemp_ = coolantTemp;
    overheatDtcActive_ = overheatDtcActive;
}

std::vector<uint8_t> DiagnosticManager::HandleUdsRequest(const std::vector<uint8_t>& request) const {
    if (request.size() < 3U) {
        return {kUdsNegativeRespSid, request.empty() ? 0x00U : request[0], 0x13U}; /* IncorrectMessageLength */
    }

    const uint8_t sid = request[0];
    if (sid == kUdsReadDataById) {
        const uint16_t did = (static_cast<uint16_t>(request[1]) << 8U) | static_cast<uint16_t>(request[2]);

        if (did == kDidCoolantTemp) {
            return {
                static_cast<uint8_t>(sid + kUdsPositiveRespOffset),
                request[1],
                request[2],
                static_cast<uint8_t>(latestCoolantTemp_ & 0xFF)
            };
        }

        if (did == kDidOverheatDtcStatus) {
            return {
                static_cast<uint8_t>(sid + kUdsPositiveRespOffset),
                request[1],
                request[2],
                static_cast<uint8_t>(overheatDtcActive_ ? 0x01U : 0x00U)
            };
        }

        return {kUdsNegativeRespSid, sid, 0x31U}; /* RequestOutOfRange */
    }

    return {kUdsNegativeRespSid, sid, 0x11U}; /* ServiceNotSupported */
}

} // namespace diag
} // namespace ara
