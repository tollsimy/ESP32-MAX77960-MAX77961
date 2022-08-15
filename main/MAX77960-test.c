#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ESP32_MAX77960.h"

static const char *TAG = "Demo-MAX77960";

#define SDA_PIN (27)
#define SCL_PIN (32)
#define I2C_PORT (0)

I2C_CONF={
    .mode = I2C_MODE_MASTER;
    .sda_io_num = SDA_PIN;
    .scl_io_num = SCL_PIN;
    .sda_pullup_en = GPIO_PULLUP_DISABLE;     //disable if you have external pullup
    .scl_pullup_en = GPIO_PULLUP_DISABLE;
    .master.clk_speed = 400000;               //I2C Full Speed
}

ESP32_MAX77960 MAX77960={0};

/* WARNING: Remember to set all fields, otherwise they'll be set to 0! */
MAX77960_CHG_CONFIG_t MAX_CHG_CONF={
    .INLIM=MAX77960_CHGIN_ILIM_1500MA,
    .INLIM_CLK_=MAX77960_INLIM_CLK_1024_US,
    .OTG_ILIM_=MAX77960_OTG_ILIM_500MA,
    .ISET=MAX77960_CHG_CHGCC_1000MA,
    .ITO=MAX77960_TO_ITH_200MA,
    .VSET_2S=MAX77960_CHG_CV_PRM_2S_8_40V,
    .VSET_3S=MAX77960_CHG_CV_PRM_3S_12_60V,
    .I_TRICKLE=MAX77960_ITRICKLE_100MA,
    .TO_timer=MAX77960_TO_TIME_30MIN,
    .FC_timer=MAX77960_FCHGTIME_3HRS,
    .CHG_RST=MAX77960_CHG_RSTRT_200MV,
    .JEITA_enabled=true
};

/* WARNING: Remember to set all fields, otherwise they'll be set to 0! */
MAX77960_CONFIG_t MAX_CONF={
    .B2SOVRC_=MAX77960_B2SOVRC_3_0A,
    .MINVSYS_2S=MAX77960_MINVSYS_2S_6970MV,     //ITRICKLE untill 3.2V per cell
    .MINVSYS_3S=MAX77960_MINVSYS_3S_10148MV,    //ITRICKLE untill 3.2V per cell
    .REGTEMP_=MAX77960_REGTEMP_115_C,
    .VCHGIN_REG_=MAX77960_VCHGIN_REG_4725MV     //Min voltage for CHGIN (VBUS)
};

MAX77960_DETAILS_STRINGS_t DETAILS_STRINGS={0};

void MAX77960_task(){
    for(;;){
        DETAILS_STRINGS=MAX_get_details(&MAX77960);
        ESP_LOGI(TAG, "MAX77960 Details:\n\tCHGIN: %s\n\tOTG: %s\n\tBAT: %s\n\tCHG: %s\n\tTHM: %s\n\tMODE: %s\n\tQBAT: %s\n\tCharger: %s", \
            DETAILS_STRINGS.CHGIN_det_s, DETAILS_STRINGS.OTG_det_s,\
            DETAILS_STRINGS.BAT_det_s, DETAILS_STRINGS.CHG_det_s, \
            DETAILS_STRINGS.THM_det_s, DETAILS_STRINGS.mode_s, \
            DETAILS_STRINGS.QB_enabled_s, DETAILS_STRINGS.CHG_enabled_s);
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
    //MAX_charge_protection_unlock(&MAX77960);
    //MAX_set_mode(&MAX77960, MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_ON);
    //MAX_charger_protection_lock(&MAX77960);
    vTaskDelete(NULL);
}

void app_main(void)
{   
    //Install I2C Driver
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &(I2C_CONF)));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    MAX_init(&MAX77960, I2C_PORT);
    ESP_LOGI(TAG, "MAX77960 Init complete!");
    MAX_charger_protection_unlock(&MAX77960);
    MAX_set_chg_conf(&MAX77960, MAX_CHG_CONF);
    MAX_set_conf(&MAX77960, MAX_CONF);
    //Smart Power Selector
    MAX_set_mode(&MAX77960, MAX77960_MODE_CHG_ON_OTG_OFF_DCDC_ON_0);
    /* Enable I2C mode since resistors values are programmed to be invalid for
    security reasons */
    MAX_set_chg_i2c_mode(&MAX77960);
    MAX_charger_protection_lock(&MAX77960);

    xTaskCreate(&MAX77960_task, "MAX77960_task", 4096, NULL, 5, NULL);ù
    
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_PORT));
}

