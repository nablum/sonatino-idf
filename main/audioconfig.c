// audioconfig.c
#include "audioconfig.h"
#include "esp_log.h"

static const char *TAG = "AUDIO_CONFIG";

i2s_chan_handle_t tx_chan = NULL;
i2s_chan_handle_t rx_chan = NULL;

esp_err_t audio_init(void)
{
    // === 1. Create TX and RX channels ===
    i2s_chan_config_t chan_cfg_tx = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    i2s_chan_config_t chan_cfg_rx = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg_tx, &tx_chan, NULL));
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg_rx, NULL, &rx_chan));

    // === 2. DAC (TX) config ===
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

    // === 3. ADC (RX) config ===
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

    // === 4. Initialize and enable ===
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg_tx));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_chan, &std_cfg_rx));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));

    ESP_LOGI(TAG, "Audio I2S initialized (48 kHz, 16-bit, master mode)");
    return ESP_OK;
}
