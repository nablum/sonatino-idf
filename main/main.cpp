// main.cpp
#include <cstdlib>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Include C audio I2S setup
extern "C" {
    #include "audioconfig.h"
}

// DaisySP
#include "daisysp.h"
using namespace daisysp;

static const char* TAG = "MAIN_DSP";

static Overdrive drive;
static float pre_gain  = 0.25f;
static float drive_amt = 0.1f;
static float post_gain = 0.5f;

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(audio_init());

    // Initialize DaisySP effect
    drive.Init();
    drive.SetDrive(drive_amt);
    ESP_LOGI(TAG, "DaisySP Overdrive ready");

    int16_t* adc_buf = (int16_t*)malloc(DMA_BUF_LEN * sizeof(int16_t));
    int16_t* dac_buf = (int16_t*)malloc(DMA_BUF_LEN * 2 * sizeof(int16_t));
    if (!adc_buf || !dac_buf) {
        ESP_LOGE(TAG, "malloc failed");
        vTaskDelete(NULL);
    }

    while (true) {
        size_t bytes_read = 0;
        if (i2s_channel_read(rx_chan, adc_buf, DMA_BUF_LEN * sizeof(int16_t),
                             &bytes_read, portMAX_DELAY) != ESP_OK)
            continue;

        size_t samples = bytes_read / sizeof(int16_t);

        for (size_t i = 0; i < samples; ++i) {
            float x = (float)adc_buf[i] / 32768.0f;
            x *= pre_gain;
            x = drive.Process(x);
            x *= post_gain;
            if (x > 1.0f) x = 1.0f;
            if (x < -1.0f) x = -1.0f;
            int16_t y = (int16_t)(x * 32767.0f);
            dac_buf[2*i + 0] = y;
            dac_buf[2*i + 1] = y;
        }

        size_t bytes_written = 0;
        i2s_channel_write(tx_chan, dac_buf, samples * 2 * sizeof(int16_t),
                          &bytes_written, portMAX_DELAY);
    }
}
