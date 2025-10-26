# Register main component
idf_component_register(
    SRCS 
        "main.cpp"
        "audioconfig.c"
    INCLUDE_DIRS 
        "."
        "${CMAKE_CURRENT_SOURCE_DIR}"
    REQUIRES 
        driver
        esp_driver_i2s
        daisysp
)

# Apply warning suppression flags as a list
set(EXTRA_CXX_FLAGS "-Wno-ignored-qualifiers;-Wno-error=ignored-qualifiers")
target_compile_options(${COMPONENT_LIB} PRIVATE ${EXTRA_CXX_FLAGS})