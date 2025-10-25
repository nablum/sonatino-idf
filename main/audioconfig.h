// audioconfig.h
#pragma once

#include "driver/i2s_std.h"
#include "esp_err.h"

// Common audio parameters
#define SAMPLE_RATE     48000
#define BITS_PER_SAMPLE 16
#define DMA_BUF_LEN     512
#define DMA_BUF_COUNT   8

// Pin mapping
#define MCLK_PIN        0
#define DAC_BCLK_PIN    15
#define DAC_LRCLK_PIN   16
#define DAC_DATA_PIN    17
#define ADC_BCLK_PIN    38
#define ADC_LRCLK_PIN   39
#define ADC_DATA_PIN    40

// Global I2S channel handles
extern i2s_chan_handle_t tx_chan;
extern i2s_chan_handle_t rx_chan;

// Function prototypes
esp_err_t audio_init(void);
