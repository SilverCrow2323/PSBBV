cmake_minimum_required(VERSION 3.16)

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

set(VITA_APP_NAME "PSBBV")
set(VITA_TITLEID "SCBV00001")
set(VITA_VERSION "00.10")

add_executable(PSBBV
    src/main.cpp
    src/app/App.cpp
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
    vita2d
    SceTouch_stub
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
