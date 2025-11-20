# ESP-IDF wrapper for upstream DaisySP
cmake_minimum_required(VERSION 3.16)

# Path to the upstream DaisySP cloned as a submodule and its patched version
set(DAISYSP_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../third_party/daisysp)
set(PATCHED_DAISYSP_ROOT ${CMAKE_BINARY_DIR}/patch/daisysp)

# Patch DaisySP library into build/patch/daisysp
execute_process(
    COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/patch.py ${DAISYSP_ROOT} DSY_REVERBSC_MAX_SIZE
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE PATCH_RESULT
    OUTPUT_VARIABLE PATCH_OUTPUT
    ERROR_VARIABLE PATCH_ERROR
)
if(NOT PATCH_RESULT EQUAL 0)
    message(FATAL_ERROR "patch.py failed:\n${PATCH_OUTPUT}\n${PATCH_ERROR}")
endif()
message(STATUS "${PATCH_OUTPUT}")

# Register DaisySP component with ESP-IDF
idf_component_register(
    SRCS
    ${PATCHED_DAISYSP_ROOT}/DaisySP-LGPL/Source/Effects/reverbsc.cpp
    INCLUDE_DIRS
    ${PATCHED_DAISYSP_ROOT}/Source
    ${PATCHED_DAISYSP_ROOT}/DaisySP-LGPL/Source/
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
