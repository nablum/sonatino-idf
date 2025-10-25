#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "audioconfig.h"

static const char *TAG = "MAIN_AUDIO";
static float audio_gain = 0.5f;  // Adjustable gain

void audio_loopback_task(void *arg)
{
    int16_t *adc_buf = malloc(DMA_BUF_LEN * sizeof(int16_t));
    int16_t *dac_buf = malloc(DMA_BUF_LEN * 2 * sizeof(int16_t)); // stereo
    if (!adc_buf || !dac_buf) {
        ESP_LOGE(TAG, "malloc failed");
        vTaskDelete(NULL);
    }

    while (1) {
        size_t bytes_read = 0;
        if (i2s_channel_read(rx_chan, adc_buf, DMA_BUF_LEN * sizeof(int16_t),
                             &bytes_read, portMAX_DELAY) != ESP_OK)
            continue;

        size_t samples = bytes_read / sizeof(int16_t);
        for (size_t i = 0; i < samples; i++) {
            float sample = (float)adc_buf[i] * audio_gain;
            if (sample > 32767.0f) sample = 32767.0f;
            if (sample < -32768.0f) sample = -32768.0f;
            int16_t out = (int16_t)sample;

            dac_buf[i * 2 + 0] = out; // L
            dac_buf[i * 2 + 1] = out; // R
        }

        size_t bytes_written = 0;
        i2s_channel_write(tx_chan, dac_buf, samples * 2 * sizeof(int16_t),
                          &bytes_written, portMAX_DELAY);
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(audio_init());
    ESP_LOGI(TAG, "Starting audio loopback task with gain=%.2f", audio_gain);
    xTaskCreatePinnedToCore(audio_loopback_task, "audio_loopback_task",
                            8192, NULL, 5, NULL, 0);
}
