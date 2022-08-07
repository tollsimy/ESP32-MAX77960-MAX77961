#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ESP32_MAX77960.h"

uint8_t MAX_SDA_PIN=18;
uint8_t MAX_SCL_PIN=19;
uint8_t MAX_I2C_PORT=0;

static const char *TAG = "Demo-MAX77960";

void MAX77960_task(){
    for(;;){
        // ESP32_MAX77960 MAX77960={0};
        // ESP_LOGI(TAG, "MAX77960 task started");

        // ESP_ERROR_CKECK(AS_init(&MAX77960));
        // MAX_enable(&MAX77960);

        // ESP_ERROR_CKECK(MAX_delete());
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{   
    xTaskCreate(&MAX77960_task, "MAX77960_task", 4096, NULL, 5, NULL);
}

