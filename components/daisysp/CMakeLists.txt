# ESP-IDF wrapper for upstream DaisySP

# Path to the upstream DaisySP cloned as a submodule
set(DAISYSP_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../third_party/daisysp)

# Collect sources
file(GLOB_RECURSE DAISYSP_SRCS
    "${DAISYSP_ROOT}/Source/*.cpp"
    "${DAISYSP_ROOT}/Source/*.c"
)

# Register DaisySP component with ESP-IDF
idf_component_register(
    SRCS ${DAISYSP_SRCS}
    INCLUDE_DIRS
        ${DAISYSP_ROOT}/Source          # for includes like "Synthesis/oscillator.h"
        ${DAISYSP_ROOT}/Source/Utility  # so "dsp.h" is found (included as "dsp.h")
)

# Quiet a couple of upstream warnings
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-extra
    -Wno-enum-compare
)
