# Register main component with ESP-IDF
idf_component_register(
    SRCS
        "main.cpp"
        "audioconfig.cpp"
    INCLUDE_DIRS
        "."
    REQUIRES
        driver
        esp_driver_i2s
        esp_driver_gpio
        daisysp
)

# Relax compiler flags
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-ignored-qualifiers
    -Wno-extra
)