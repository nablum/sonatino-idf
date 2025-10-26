# ESP-IDF wrapper that builds DaisySP (unmodified) from third_party/
# and adds a tiny shim include path that provides dsp.h

set(DAISYSP_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../third_party/daisysp)

# Collect upstream sources
file(GLOB_RECURSE DAISYSP_SRCS
    "${DAISYSP_ROOT}/Source/*.cpp"
    "${DAISYSP_ROOT}/Source/*.c"
)

idf_component_register(
    SRCS ${DAISYSP_SRCS}
    INCLUDE_DIRS
        ${DAISYSP_ROOT}/Source    # upstream headers
        ${CMAKE_CURRENT_LIST_DIR}/shim  # our shim with dsp.h
)

# Optional: silence specific upstream warnings without touching your code
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-extra
    -Wno-enum-compare
)
