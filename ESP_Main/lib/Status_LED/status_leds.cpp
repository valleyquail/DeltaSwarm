#include "status_leds.h"

#include "../../include/espidf_rmt/led_strip_encoder.h"
#include "driver/rmt_tx.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)


// Configurations
//______________________________________________________________________________________________________________________
rmt_channel_handle_t led_channel;
rmt_encoder_handle_t led_encoder;
rmt_transmit_config_t tx_config;
//______________________________________________________________________________________________________________________
const uint8_t NUMPIXELS = 2;
//______________________________________________________________________________________________________________________
// For some reason these are stored as GRB Values it seems
uint8_t led_color_values[3 * NUMPIXELS];

void initStatusLED(int8_t pin)
{
    rmt_tx_channel_config_t tx_chan_config = {
        .gpio_num = (gpio_num_t)pin,
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .mem_block_symbols = 64, // increase the block size can make the LED less flickering
        .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_channel));

    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));
    ESP_ERROR_CHECK(rmt_enable(led_channel));

    tx_config.loop_count = 0; // no transfer loop
}

// Set the LED to red
void statusLEDSetError()
{
    led_color_values[0] = 0;
    led_color_values[1] = 100;
    led_color_values[2] = 0;

    ESP_ERROR_CHECK(rmt_transmit(led_channel, led_encoder, led_color_values, sizeof(led_color_values), &tx_config));

    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_channel, portMAX_DELAY));
}

// Set the LED to yellow
void statusLEDSetWarning()
{
    led_color_values[0] = 100;
    led_color_values[1] = 100;
    led_color_values[2] = 0;

    ESP_ERROR_CHECK(rmt_transmit(led_channel, led_encoder, led_color_values, sizeof(led_color_values), &tx_config));

    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_channel, portMAX_DELAY));
}

// Set the LED to green
void statusLEDSetOK()
{
    led_color_values[0] = 100;
    led_color_values[1] = 0;
    led_color_values[2] = 0;

    ESP_ERROR_CHECK(rmt_transmit(led_channel, led_encoder, led_color_values, sizeof(led_color_values), &tx_config));

    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_channel, portMAX_DELAY));
}

// Turn off the LED
void SetStatusLEDOff()
{
    led_color_values[0] = 0;
    led_color_values[1] = 0;
    led_color_values[2] = 0;

    ESP_ERROR_CHECK(rmt_transmit(led_channel, led_encoder, led_color_values, sizeof(led_color_values), &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_channel, portMAX_DELAY));
}


void SetBatteryLEDColor(uint8_t level)
{
    // set the battery LED to red if the battery is below 20%
    if (level < 20)
    {
        led_color_values[3] = 100;
        led_color_values[4] = 0;
        led_color_values[5] = 0;
    }
    // set the battery LED to yellow if the battery is below 50%
    else if (level < 50)
    {
        led_color_values[3] = 30;
        led_color_values[4] = 100;
        led_color_values[5] = 0;
    }
    // set the battery LED to green if the battery is above 50%
    else
    {
        led_color_values[3] = 0;
        led_color_values[4] = 100;
        led_color_values[5] = 0;
    }
}
