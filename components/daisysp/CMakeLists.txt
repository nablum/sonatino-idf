# ESP-IDF wrapper for upstream DaisySP
cmake_minimum_required(VERSION 3.16)

# Path to the upstream DaisySP cloned as a submodule
set(DAISYSP_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../daisysp)

# Collect sources
file(GLOB_RECURSE DAISYSP_SRCS 
    ${DAISYSP_ROOT}/DaisySP-LGPL/Source/Effects/reverbsc.cpp
)

# Register DaisySP component with ESP-IDF
idf_component_register(
    SRCS ${DAISYSP_SRCS}
    INCLUDE_DIRS
        ${DAISYSP_ROOT}/Source
        ${DAISYSP_ROOT}/DaisySP-LGPL/Source/
)

# Compile-time definitions
target_compile_definitions(${COMPONENT_LIB} PUBLIC 
    USE_DAISYSP_LGPL=1 
    DSY_REVERBSC_MAX_SIZE=32768
)

# Compile-time options
target_compile_options(${COMPONENT_LIB} PUBLIC
    -Os
    -Wno-extra
    -Wno-enum-compare
)