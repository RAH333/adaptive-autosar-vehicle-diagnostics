cmake_minimum_required(VERSION 3.14)
project(AdaptiveAutosarVehicleDiagnostics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_executable(adaptive_vehicle_health
    ara/com/service_registry.cpp
    ara/diag/diagnostic_manager.cpp
    applications/powertrain_provider/powertrain_service.cpp
    applications/diag_client/vehicle_health_app.cpp
)
