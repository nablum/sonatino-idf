#pragma once

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// Common audio parameters
#define SAMPLE_RATE     48000
#define BITS_PER_SAMPLE 16
#define DMA_BUF_LEN     512
#define DMA_BUF_COUNT   8

// Pin mapping
#define MCLK_PIN        GPIO_NUM_0
#define DAC_BCLK_PIN    GPIO_NUM_15
#define DAC_LRCLK_PIN   GPIO_NUM_16
#define DAC_DATA_PIN    GPIO_NUM_17
#define ADC_BCLK_PIN    GPIO_NUM_38
#define ADC_LRCLK_PIN   GPIO_NUM_39
#define ADC_DATA_PIN    GPIO_NUM_40

// Global I2S channel handles
extern i2s_chan_handle_t tx_chan;
extern i2s_chan_handle_t rx_chan;

// Function prototype
esp_err_t audio_init(void);

#ifdef __cplusplus
}
#endif
