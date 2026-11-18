cmake_minimum_required(VERSION 3.16)

option(PSBBV_BUILD_HOST_TESTS_ONLY "Build portable PSBBV core tests without VitaSDK" OFF)
if(PSBBV_BUILD_HOST_TESTS_ONLY)
    project(PSBBVHostTests LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    enable_testing()

    add_executable(psbbv_core_tests
        tests/ChannelRegistryTests.cpp
        src/navigation/ChannelRegistry.cpp
        src/nodes/NodeRegistry.cpp
        src/nodes/NodeManifest.cpp
        src/nodes/NodeLifecycle.cpp
        src/nodes/NodeJob.cpp
    )
    target_include_directories(psbbv_core_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_core_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_channel_registry COMMAND psbbv_core_tests)

    add_executable(psbbv_ui_state_tests
        tests/UiStateTests.cpp
        src/navigation/UiState.cpp
        src/navigation/ChannelRegistry.cpp
    )
    target_include_directories(psbbv_ui_state_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_ui_state_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_ui_state COMMAND psbbv_ui_state_tests)

    add_executable(psbbv_node_registry_tests
        tests/NodeRegistryTests.cpp
        src/nodes/NodeRegistry.cpp
    )
    target_include_directories(psbbv_node_registry_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_node_registry_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_node_registry COMMAND psbbv_node_registry_tests)

    add_executable(psbbv_node_manifest_tests
        tests/NodeManifestTests.cpp
        src/nodes/NodeManifest.cpp
        src/nodes/NodeRegistry.cpp
    )
    target_include_directories(psbbv_node_manifest_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_node_manifest_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_node_manifest COMMAND psbbv_node_manifest_tests)

    add_executable(psbbv_node_lifecycle_tests
        tests/NodeLifecycleTests.cpp
        src/nodes/NodeLifecycle.cpp
    )
    target_include_directories(psbbv_node_lifecycle_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_node_lifecycle_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_node_lifecycle COMMAND psbbv_node_lifecycle_tests)

    add_executable(psbbv_node_job_tests
        tests/NodeJobTests.cpp
        src/nodes/NodeJob.cpp
    )
    target_include_directories(psbbv_node_job_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_node_job_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_node_jobs COMMAND psbbv_node_job_tests)

    add_executable(psbbv_games_browser_tests
        tests/GamesBrowserStateTests.cpp
        src/navigation/GamesBrowserState.cpp
    )
    target_include_directories(psbbv_games_browser_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_games_browser_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_games_browser COMMAND psbbv_games_browser_tests)

    add_executable(psbbv_games_discovery_lifecycle_tests
        tests/GamesDiscoveryLifecycleTests.cpp
    )
    target_include_directories(psbbv_games_discovery_lifecycle_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_games_discovery_lifecycle_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_games_discovery_lifecycle COMMAND psbbv_games_discovery_lifecycle_tests)

    add_executable(psbbv_touch_mapping_tests
        tests/TouchMappingTests.cpp
    )
    target_include_directories(psbbv_touch_mapping_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_compile_options(psbbv_touch_mapping_tests PRIVATE -Wall -Wextra -Wpedantic)

    add_test(NAME psbbv_touch_mapping COMMAND psbbv_touch_mapping_tests)
    return()
endif()

# Resolve the VitaSDK toolchain from the standard environment variable.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE FILEPATH "VitaSDK toolchain file")
    else()
        message(FATAL_ERROR "VITASDK is not set. Install VitaSDK and export VITASDK before configuring PSBBV.")
    endif()
endif()

project(PSBBV VERSION 0.1.0 LANGUAGES C CXX)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Pin the independently maintained Games node to a reviewed commit.
# Developers may point this at a local checkout without changing the pin.
include(FetchContent)
set(PSBBV_GAMES_GIT_TAG "a2f331dd58e288fd1d9e47b4502d39e54bfbae34"
    CACHE STRING "Reviewed PSBBV-Games commit")
set(PSBBV_GAMES_SOURCE_DIR "" CACHE PATH "Optional local PSBBV-Games checkout")
set(PSBBV_GAMES_BUILD_HOST_TESTS OFF CACHE BOOL "Do not cross-build host tests" FORCE)
set(PSBBV_GAMES_BUILD_VITA_ADAPTER ON CACHE BOOL "Build the Vita filesystem provider" FORCE)

if(PSBBV_GAMES_SOURCE_DIR)
    add_subdirectory("${PSBBV_GAMES_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/psbbv-games")
else()
    FetchContent_Declare(psbbv_games
        GIT_REPOSITORY https://github.com/SilverCrow2323/PSBBV-Games.git
        GIT_TAG ${PSBBV_GAMES_GIT_TAG}
        GIT_SHALLOW FALSE
    )
    FetchContent_MakeAvailable(psbbv_games)
endif()

set(VITA_APP_NAME "PSBBV")
set(VITA_TITLEID "SCBV00001")
set(VITA_VERSION "00.10")

add_executable(PSBBV
    src/main.cpp
    src/app/App.cpp
    src/navigation/ChannelRegistry.cpp
    src/navigation/UiState.cpp
    src/navigation/GamesBrowserState.cpp
    src/nodes/NodeRegistry.cpp
    src/nodes/NodeManifest.cpp
    src/nodes/NodeLifecycle.cpp
    src/nodes/NodeJob.cpp
)

target_include_directories(PSBBV PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_options(PSBBV PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fno-rtti
    -fno-exceptions
)

target_link_libraries(PSBBV PRIVATE
    psbbv_games_vita_adapter
    vita2d
    SceDisplay_stub
    SceGxm_stub
    SceSysmodule_stub
    SceCtrl_stub
    SceTouch_stub
    ScePgf_stub
    ScePvf_stub
    SceCommonDialog_stub
    SceAppMgr_stub
    SceKernelThreadMgr_stub
    freetype
    png
    jpeg
    z
    m
    stdc++
)

vita_create_self(PSBBV.self PSBBV)

vita_create_vpk(PSBBV.vpk ${VITA_TITLEID} PSBBV.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
    FILE sce_sys/icon0.png sce_sys/icon0.png
    FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
    FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
    FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
