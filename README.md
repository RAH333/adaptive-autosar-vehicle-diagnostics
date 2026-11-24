# adaptive-autosar-vehicle-diagnostics
Adaptive AUTOSAR (C++17) Service-Oriented Architecture (SOA) Vehicle Health &amp; Diagnostics Application implementing ara::com (SOME/IP-style service proxy/skeleton), ara::diag (UDS), CMake, Python SIL verification, and GitHub Actions CI/CD.




# Adaptive AUTOSAR Service-Oriented Vehicle Diagnostics (`ara::com` & `ara::diag`)

A modern **C++17 Adaptive AUTOSAR** prototype demonstrating **Service-Oriented Architecture (SOA)** communication (`ara::com`) and **UDS Diagnostics** (`ara::diag`) between a Powertrain Telemetry Provider and a Vehicle Health Diagnostic Client, complete with **CMake**, **Python SIL automation**, and **GitHub Actions CI/CD**.

## Architecture Highlights
* **`ara::com` Service Discovery & Pub/Sub (`ara/com/`):** Implements thread-safe service registration (`OfferService` / `FindService`) and event-driven telemetry streaming.
* **`ara::diag` UDS Handler (`ara/diag/`):** Processes ISO 14229 UDS requests (`0x22 ReadDataByIdentifier`) to expose real-time coolant temperature and thermal DTC status.
* **Adaptive Applications (`applications/`):** Decoupled C++17 provider skeleton and diagnostic client executables.
* **Automated SIL Verification (`sil_tests/`):** Validates service discovery, threshold fault triggering, and hex UDS payloads in CI.

## Build & Run Locally
```bash
cmake -B build -S .
cmake --build build
python3 sil_tests/test_adaptive_soa.py
```





```
adaptive-autosar-vehicle-diagnostics/
├── .github/
│   └── workflows/
│       └── adaptive_ci.yml
├── ara/
│   ├── com/
│   │   ├── service_registry.hpp
│   │   └── service_registry.cpp
│   └── diag/
│       ├── diagnostic_manager.hpp
│       └── diagnostic_manager.cpp
├── applications/
│   ├── powertrain_provider/
│   │   ├── powertrain_service.hpp
│   │   └── powertrain_service.cpp
│   └── diag_client/
│       └── vehicle_health_app.cpp
├── sil_tests/
│   └── test_adaptive_soa.py
├── CMakeLists.txt
└── README.md
```
