# Top-level CMake
cmake_minimum_required(VERSION 3.16)

# IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Build components
set(COMPONENTS
    # App and local components
    main
    daisysp

    # core runtime
    freertos
    esp_system
    esp_common
    esp_hw_support
    hal
    soc
    esp_rom
    esp_timer
    heap
    log
    esp_libc      # (IDF 6.x) replaces "newlib" naming from older IDF
    cxx           # C++ runtime

    # drivers
    esp_driver_i2s
    esp_driver_gpio

    # small helper deps some drivers pull
    esp_ringbuf
    xtensa        # arch helper component
    esp_pm        # safe to include; some drivers reference PM APIs
)

# Set the project name and version
project(sonatino-idf VERSION 0.0.1)
