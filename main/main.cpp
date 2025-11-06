// main.cpp
#include <cstdlib>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// C audio I2S setup
extern "C" {
    #include "audioconfig.h"
}

// DaisySP library
#include "daisysp.h"
using namespace daisysp;

static const char* TAG = "MAIN_DSP";

// -------- Reverb parameters --------
static ReverbSc reverb;
static float    wet_mix   = 0.35f;     // 0=dry, 1=fully wet
static float    feedback  = 0.65f;     // reduced for smaller memory usage (normally 0.7-0.9)
static float    lpf_hz    = 10000.0f;  // slightly darker to mask shorter tail

// Campler
static inline float clamp1(float x) {
    if (x > 1.0f)  return 1.0f;
    if (x < -1.0f) return -1.0f;
    return x;
}

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(audio_init());

    // Init DaisySP reverb
    reverb.Init(static_cast<float>(SAMPLE_RATE));
    reverb.SetFeedback(feedback);
    reverb.SetLpFreq(lpf_hz);
    ESP_LOGI(TAG, "DaisySP ReverbSc ready (wet=%.2f, fb=%.2f, lpf=%.0f Hz)",
             wet_mix, feedback, lpf_hz);

    // DMA buffers: ADC mono, DAC stereo
    int16_t* adc_buf = (int16_t*)malloc(DMA_BUF_LEN * sizeof(int16_t));
    int16_t* dac_buf = (int16_t*)malloc(DMA_BUF_LEN * 2 * sizeof(int16_t));
    if (!adc_buf || !dac_buf) {
        ESP_LOGE(TAG, "malloc failed");
        vTaskDelete(NULL);
    }

    while (true) {
        size_t bytes_read = 0;
        if (i2s_channel_read(rx_chan, adc_buf, DMA_BUF_LEN * sizeof(int16_t),
                             &bytes_read, portMAX_DELAY) != ESP_OK) {
            continue;
        }

        const size_t samples = bytes_read / sizeof(int16_t);

        for (size_t i = 0; i < samples; ++i) {
            // Normalize mono input
            const float x  = (float)adc_buf[i] / 32768.0f;

            // Process through stereo reverb (feed mono to both inputs)
            float wetL = 0.0f, wetR = 0.0f;
            reverb.Process(x, x, &wetL, &wetR);

            // Dry/wet mix
            float outL = clamp1((1.0f - wet_mix) * x + wet_mix * wetL);
            float outR = clamp1((1.0f - wet_mix) * x + wet_mix * wetR);

            // Convert to int16 and interleave
            dac_buf[2*i + 0] = (int16_t)(outL * 32767.0f);
            dac_buf[2*i + 1] = (int16_t)(outR * 32767.0f);
        }

        size_t bytes_written = 0;
        (void)i2s_channel_write(tx_chan, dac_buf, samples * 2 * sizeof(int16_t),
                                &bytes_written, portMAX_DELAY);
    }
}
