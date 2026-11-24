import os
import subprocess
import sys

EXECUTABLE = os.path.join("build", "adaptive_vehicle_health")
if os.name == "nt":
    EXECUTABLE += ".exe"

TEST_SCENARIOS = [
    # (Scenario Name, RPM, CoolantTempC, BatteryMv, ExpectedWarn, ExpectedUdsHex)
    ("Nominal Highway Cruise",     2800,  90, 13800, 0, "62F10600"),
    ("Boundary Coolant Threshold", 3500, 105, 13600, 0, "62F10600"),
    ("Engine Overheat Fault",      4200, 114, 13200, 1, "62F10601"),
    ("Cold Start Idle",             850,  22, 12400, 0, "62F10600"),
]

def run_adaptive_node(rpm: int, temp: int, batt: int) -> dict:
    proc = subprocess.run(
        [EXECUTABLE, str(rpm), str(temp), str(batt)],
        capture_output=True,
        text=True,
        check=True
    )
    for line in proc.stdout.splitlines():
        if line.startswith("ADAPTIVE_SIL|"):
            parts = line.split("|")[1:]
            return {p.split(":")[0]: p.split(":")[1] for p in parts}
    raise RuntimeError("ADAPTIVE_SIL output line not found.")

def main():
    print("=== Adaptive AUTOSAR (ara::com / ara::diag) SIL Verification ===")
    passed = 0

    for name, rpm, temp, batt, exp_warn, exp_hex in TEST_SCENARIOS:
        res = run_adaptive_node(rpm, temp, batt)
        ok = (
            res["SD_FOUND"] == "1" and
            int(res["OVERHEAT_WARN"]) == exp_warn and
            res["UDS_HEX"] == exp_hex
        )
        tag = "PASS" if ok else "FAIL"
        print(f"[{tag}] {name} (T={temp}C) -> Warn={res['OVERHEAT_WARN']}, UDS={res['UDS_HEX']}")
        if ok:
            passed += 1

    print(f"\nResult: {passed}/{len(TEST_SCENARIOS)} Adaptive SOA Scenarios Passed.")
    sys.exit(0 if passed == len(TEST_SCENARIOS) else 1)

if __name__ == "__main__":
    main()
  
