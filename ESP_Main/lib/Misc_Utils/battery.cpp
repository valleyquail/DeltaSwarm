#include "battery.h"

#include "FreeRTOS.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"
#include "../../include/config.h"

void initBatteryADC()
{
    // Configure ADC
    printf("Configuring ADC\n");
}
