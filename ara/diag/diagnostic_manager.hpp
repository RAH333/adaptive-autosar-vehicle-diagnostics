#ifndef ARA_DIAG_DIAGNOSTIC_MANAGER_HPP
#define ARA_DIAG_DIAGNOSTIC_MANAGER_HPP

#include <cstdint>
#include <vector>

namespace ara {
namespace diag {

/* ISO 14229 UDS Constants */
constexpr uint8_t  kUdsReadDataById       = 0x22U;
constexpr uint8_t  kUdsPositiveRespOffset = 0x40U;
constexpr uint8_t  kUdsNegativeRespSid    = 0x7FU;
constexpr uint16_t kDidCoolantTemp        = 0xF105U;
constexpr uint16_t kDidOverheatDtcStatus  = 0xF106U;

class DiagnosticManager {
public:
    DiagnosticManager();

    void UpdateVehicleState(int16_t coolantTemp, bool overheatDtcActive);
    std::vector<uint8_t> HandleUdsRequest(const std::vector<uint8_t>& request) const;

private:
    int16_t latestCoolantTemp_;
    bool    overheatDtcActive_;
};

} // namespace diag
} // namespace ara

#endif // ARA_DIAG_DIAGNOSTIC_MANAGER_HPP
