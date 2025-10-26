#include "audioconfig.h"
#include "esp_log.h"
#include "esp_check.h"
#include <cstring>

static const char *TAG = "AUDIO_CONFIG";

// Global I2S handles
i2s_chan_handle_t tx_chan = nullptr;
i2s_chan_handle_t rx_chan = nullptr;

esp_err_t audio_init(void)
{
    // === 1. Create TX (DAC) and RX (ADC) channels ===
    i2s_chan_config_t chan_cfg_tx = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    i2s_chan_config_t chan_cfg_rx = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_MASTER);

    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan_cfg_tx, &tx_chan, nullptr), TAG, "TX channel creation failed");
    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan_cfg_rx, nullptr, &rx_chan), TAG, "RX channel creation failed");

    // === 2. Configure DAC (TX) ===
    i2s_std_config_t std_cfg_tx = {
        .clk_cfg = {
            .sample_rate_hz = SAMPLE_RATE,
            .clk_src = I2S_CLK_SRC_DEFAULT,
            .mclk_multiple = I2S_MCLK_MULTIPLE_256,
        },
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_16BIT,
            .slot_mode = I2S_SLOT_MODE_STEREO,
            .slot_mask = I2S_STD_SLOT_BOTH,
            .ws_width = I2S_DATA_BIT_WIDTH_16BIT,
            .ws_pol = false,
            .bit_shift = true,
            .left_align = false,
            .big_endian = false,
            .bit_order_lsb = false,
        },
        .gpio_cfg = {
            .mclk = MCLK_PIN,
            .bclk = DAC_BCLK_PIN,
            .ws   = DAC_LRCLK_PIN,
            .dout = DAC_DATA_PIN,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };

    // === 3. Configure ADC (RX) ===
    i2s_std_config_t std_cfg_rx = {
        .clk_cfg = {
            .sample_rate_hz = SAMPLE_RATE,
            .clk_src = I2S_CLK_SRC_DEFAULT,
            .mclk_multiple = I2S_MCLK_MULTIPLE_256,
        },
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_16BIT,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .slot_mask = I2S_STD_SLOT_LEFT,
            .ws_width = I2S_DATA_BIT_WIDTH_16BIT,
            .ws_pol = false,
            .bit_shift = true,
            .left_align = false,
            .big_endian = false,
            .bit_order_lsb = false,
        },
        .gpio_cfg = {
            .mclk = MCLK_PIN,
            .bclk = ADC_BCLK_PIN,
            .ws   = ADC_LRCLK_PIN,
            .dout = I2S_GPIO_UNUSED,
            .din  = ADC_DATA_PIN,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };

    // === 4. Initialize & enable both channels ===
    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(tx_chan, &std_cfg_tx), TAG, "TX init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(rx_chan, &std_cfg_rx), TAG, "RX init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(tx_chan), TAG, "TX enable failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(rx_chan), TAG, "RX enable failed");

    ESP_LOGI(TAG, "Audio I2S initialized (48 kHz, 16-bit, master mode)");
    return ESP_OK;
}
