#include "ESP32_MAX77960.h"
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

static const char* TAG="ESP32_MAX77960";

// ---------------- I2C AUXILIARY FUNCTIONS ----------------

/**
 *  @brief  write an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
static void write8(uint8_t reg, uint8_t value) {
  
    uint8_t buffer[2] = {reg, value};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 2, 1000 / portTICK_PERIOD_MS));
}

/**
 *  @brief  write a 16 bit value over I2C
 *  @param  reg
 *  @param  value
 */
static void write16(uint8_t reg, uint16_t value) {
  
    uint8_t buffer[3] = {reg, (uint8_t)(value), (uint8_t)(value >> 8)};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 3, 1000 / portTICK_PERIOD_MS));
}

/**
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
static uint8_t read8(uint8_t reg) {
    uint8_t buffer[1] = {reg};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    return buffer[0];
}

/**
 *  @brief  Reads a 16 bit value over I2C
 *  @param  reg
 *  @return value
 */
static uint16_t read16(uint8_t reg) {
    uint8_t buffer[2] = {reg, 0};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 2, 1000 / portTICK_PERIOD_MS));
    return buffer[0] | (buffer[1] << 8);
}

/**
 *  @brief  Reads <SIZE> bytes over I2C
 *  @param  reg
 *  @param  data pointer to the data buffer
 *  @param  size number of bytes to read
 *  @return value
 */
static void read_data(uint8_t reg, uint8_t *data, size_t size) {
    uint8_t buffer[1] = {reg};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(MAX_I2C_PORT, MAX77960_I2C_ADDR, data, size, 1000 / portTICK_PERIOD_MS));
}

/**
 * @brief Write bits in a register
 * 
 * @param reg register to write to
 * @param mask mask to apply
 * @param value value to write
 */
static void writeBits(uint8_t reg, uint8_t mask, uint8_t value) {
    uint8_t tmp = read8(reg);
    tmp &= ~mask;
    tmp |= (value & mask);
    write8(reg, tmp);
}

// ---------------- PERIPHERALS GENERAL INFO GETTERS ----------------

static void max77960_get_number_of_cell(ESP32_MAX77960* MAX){
    uint8_t value = read8(CHG_DTLS_02_REG);
    MAX->NUM_CELL = (value & NUM_CELL_DTLS_MASK);
}

static void max77960_get_switching_frequency(ESP32_MAX77960* MAX){
    uint8_t value = read8(CHG_DTLS_02_REG);
    MAX->FSW = (value & FSW_DTLS_MASK) >> FSW_DTLS;
}

// ---------------- INITIALIZATION FUNCTIONS ----------------

/**
 *  @brief  Initialize the MAX77960 peripheral
 */
void max77960_init(ESP32_MAX77960* MAX) {
    if(MAX->_is_init!=true){
        max77960_get_number_of_cell(MAX);
        max77960_get_switching_frequency(MAX);
        ESP_LOGI(TAG, "MAX77960 initialized: %d cells max, %d Hz switching frequency", MAX->NUM_CELL, MAX->FSW);
        if(MAX->NUM_CELL==1){
            ESP_LOGI(TAG, "MAX77960 supports 3S mode but it's disabled by default for safety reasons, you can enable it manually");
        }

        //Set I2C configuration
        MAX->conf.mode = I2C_MODE_MASTER;
        MAX->conf.sda_io_num = MAX_SDA_PIN;
        MAX->conf.scl_io_num = MAX_SCL_PIN;
        MAX->conf.sda_pullup_en = GPIO_PULLUP_DISABLE;     //disable if you have external pullup
        MAX->conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
        MAX->conf.master.clk_speed = 400000;               //I2C Full Speed
        ESP_ERROR_CHECK(i2c_param_config(MAX_I2C_PORT, &(MAX->conf))); //set I2C Config
        ESP_ERROR_CHECK(i2c_driver_install(MAX_I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

        MAX->_is_init=true;
    }
}

void max77960_delete(ESP32_MAX77960 * MAX){
    if(MAX->_is_init==true){
        i2c_driver_delete(MAX_I2C_PORT);
        MAX->_is_init=false;
    }
    else{
        ESP_LOGE(TAG, "MAX77960 is not initialized");
    }
}

MAX77960_DETAILS_t max77960_get_details(ESP32_MAX77960* MAX){
    
}

void max77960_set_3S_mode(ESP32_MAX77960* MAX, bool enabled){
    if(MAX->_is_init=true){
        if(MAX->NUM_CELL==1){
            MAX->BATT_3S_enabled=enabled;
        }
        else{
            ESP_LOGE(TAG, "MAX77960 doesn't support 3S mode");
        }
    }
}

void max77960_set_chg_conf(ESP32_MAX77960* MAX, MAX77960_CHG_CONFIG_t chg_conf){

}

void max77960_set_conf(ESP32_MAX77960* MAX, MAX77960_CONFIG_t conf){

}


// /*
//  *	Unlock charger protection.
//  */
// MAX77960_Status_t max77960_charger_protection_unlock()
// {
// 	const uint8_t MAX_TRY = 10;
// 	uint8_t ntry = 0;
// 	uint8_t reg_data = 0;
// 	uint8_t chgprot_state = 0;

// 	while (1) {
// 		reg_write(CHG_CNFG_06_REG, CHGPROT_UNLOCK << CHGPROT);
// 		reg_read(CHG_CNFG_06_REG, &reg_data);
// 		chgprot_state = (reg_data & CHGPROT_MASK) >> CHGPROT;
// 		if (chgprot_state == CHGPROT_UNLOCK)
// 			return MAX77960_OK;
// 		else if (++ntry > MAX_TRY)
// 			return MAX77960_ERROR;
// 	}
// }

// /*
//  *	Lock charger's registers.
//  */
// MAX77960_Status_t max77960_charger_protection_lock()
// {
// 	const uint8_t MAX_TRY = 10;
// 	uint8_t ntry = 0;
// 	uint8_t reg_data = 0;
// 	uint8_t chgprot_state = 0;

// 	while (1) {
// 		reg_write(CHG_CNFG_06_REG, CHGPROT_LOCK << CHGPROT);
// 		reg_read(CHG_CNFG_06_REG, &reg_data);
// 		chgprot_state = (reg_data & CHGPROT_MASK) >> CHGPROT;
// 		if (chgprot_state == CHGPROT_LOCK)
// 			return MAX77960_OK;
// 		else if (++ntry > MAX_TRY)
// 			return MAX77960_ERROR;
// 	}
// }

// /*
//  *	Perform software reset to type O registers.
//  */
// MAX77960_Status_t max77960_software_reset()
// {
// 	if (reg_write(SWRST, SWRST_COMMAND << SWRST) != MAX77960_OK)
// 		return MAX77960_ERROR;
// 	return MAX77960_OK;
// }

// ------------- others, CHECK -----------------

// /*
//  * Copyright (c) 2019 Maxim Integrated Products, Inc.
//  * Author: Maxim Integrated <opensource@maximintegrated.com>
//  *
//  * This program is free software; you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License version 2 as
//  * published by the Free Software Foundation.
//  */

// static void max77960_charger_initialize(struct max77960_charger_data *chg_data);
// static int max77960_charger_get_health_state(struct max77960_charger_data
// 	*chg_data);

// static bool max77960_charger_unlock(struct max77960_charger_data *chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data;
// 	u8 chgprot;
// 	int retry_cnt = 0;
// 	bool need_init = false;

// 	do {
// 		max77960_read_reg(regmap, MAX77960_CHG_CNFG_06, &reg_data);
// 		chgprot = ((reg_data & 0x0C) >> 2);
// 		if (chgprot != 0x03) {
// 			pr_info("%s: Unlock err, chgprot(0x%x), retry(%d)\n",
// 				__func__,	chgprot, retry_cnt);
// 			max77960_write_reg(regmap, MAX77960_CHG_CNFG_06,
// 				(0x03 << 2));
// 			need_init = true;
// 			msleep(20);
// 		} else {
// 			pr_info("%s: Unlock success, chgprot(0x%x)\n",
// 				__func__, chgprot);
// 			break;
// 		}
// 	} while ((chgprot != 0x03) && (++retry_cnt < 10));

// 	return need_init;
// }

// static void max77960_charger_check_unlock_state(struct max77960_charger_data
// 	*chg_data)
// {
// 	bool need_reg_init;

// 	need_reg_init = max77960_charger_unlock(chg_data);
// 	if (need_reg_init) {
// 		pr_info("%s: charger locked state, reg init\n", __func__);
// 		max77960_charger_initialize(chg_data);
// 	}
// }

// static void max77960_charger_set_state(struct max77960_charger_data
// 	*chg_data, int en)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	u8 reg_data;

// 	max77960_read_reg(regmap, MAX77960_CHG_CNFG_00, &reg_data);

// 	if (en)
// 		reg_data |= MAX77960_MODE_CHGR;
// 	else
// 		reg_data &= ~MAX77960_MODE_CHGR;

// 	pr_info("%s: CHG_CNFG_00(0x%02x)\n", __func__, reg_data);

// 	max77960_write_reg(regmap, MAX77960_CHG_CNFG_00, reg_data);
// }


// static bool max77818_charger_present_input(struct max77960_charger_data
// 	*chg_data)
// {
//     u8 chg_int_ok = 0;
//     int rc;
// 	struct regmap *regmap = chg_data->regmap;

// 	rc = max77960_read_reg(regmap, MAX77960_CHG_INT_OK, &chg_int_ok);
// 	if (unlikely(IS_ERR_VALUE(rc)))
//         return false;

// 	if ((chg_int_ok & BIT_CHGIN_OK) == BIT_CHGIN_OK)
// 		return true;

// 		/* check whether charging or not in the UVLO condition */
// 		if (((chg_data->details_0 & BIT_CHGIN_DTLS) == 0) &&
// 			(((chg_data->details_1 & BIT_CHG_DTLS)
// 					== CHG_DTLS_FASTCHARGE_CC) ||
// 			 ((chg_data->details_1 & BIT_CHG_DTLS)
// 					== CHG_DTLS_FASTCHARGE_CV))) {
// 			return true;
// 	}

// 			return false;
// }

// static void max77960_charger_set_input_current(struct max77960_charger_data
// 	*chg_data, int input_current)
// {
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data = 0;
// 	int current_limit_step;

// 	current_limit_step = chg_data->input_curr_limit_step;
// 	/* unit mA */
// 	if (!input_current) {
// 		reg_data = 0;
// 	} else {
// 		if (input_current % current_limit_step)
// 	    goto err_value;
// 		if (input_current <= 100)
//       reg_data = 0;
// 	  else if (input_current <= 6300) /* 6300mA */
// 			reg_data |= ((input_current / current_limit_step)+1);
//     else
//       reg_data |= 0x7F;
//   }

// 	pr_info("max77960 set input current: reg_data(0x%02x), current(%dmA)\n",
// 		reg_data, input_current);

// 	max77960_write_reg(regmap, MAX77960_CHG_CNFG_08, reg_data);
// err_value:
// 	pr_info("max77960 set input current: %dmA\n", input_current);
// 	pr_info("max77960 not match the register.\n");
// }

// static int max77960_charger_get_input_current(struct max77960_charger_data
// 	*chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data = 0;
// 	int get_current = 0;

// 	max77960_read_reg(regmap, MAX77960_CHG_CNFG_08, &reg_data);
// 	if ((reg_data & BIT_CHGIN_ILIM) < 4)
// 		get_current = 100; 	/* 100mA */
// 	else if ((reg_data & BIT_CHGIN_ILIM) > 0x7F)
// 		get_current = 6300;	/* 6300mA */
// 	else
// 		get_current = (reg_data & BIT_CHGIN_ILIM)*50 - 50;

// 	pr_info("max77960 get input current: reg_data(0x%02x), current(%dmA)\n",
// 	  reg_data, get_current);

// 	return get_current;
// }

// static void max77960_charger_set_charge_current(struct max77960_charger_data
// 	*chg_data, int cur)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	u8 reg_data = 0;

// 	max77960_read_reg(regmap, MAX77960_CHG_CNFG_02, &reg_data);
// 	reg_data &= ~MAX77960_CHG_CC;

// 	if (!cur) {
// 		/* No charger */
// 		max77960_write_reg(regmap, MAX77960_CHG_CNFG_02, reg_data);
// 	} else {
// 		if (cur % 50)
// 	    goto err_value;
// 		if (cur > 450)
//       reg_data |= ((cur / chg_data->charging_curr_step) + 3);
// 		else
//       reg_data |= ((cur / 50) - 2);

// 		max77960_write_reg(regmap, MAX77960_CHG_CNFG_02, reg_data);
// 	}

// 	pr_info("max77960 set charge current: reg_data(0x%02x), charge(%d)\n",
// 			reg_data, cur);

// err_value:
// 	pr_info("max77960 set charge current: %dmA\n", cur);
// 	pr_info("max77960 not match the register.\n");
// }

// static int max77960_charger_get_charge_current(struct max77960_charger_data
// 	*chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	u8 reg_data;
// 	int get_current = 0;

// 	max77960_read_reg(regmap, MAX77960_CHG_CNFG_02, &reg_data);
// 	pr_info("max77960 CHG_CNFG_02(0x%02x)\n", reg_data);

// 	reg_data &= MAX77960_CHG_CC;
// 	if (reg_data > 0x07)
//     get_current = reg_data * chg_data->charging_curr_step - 300;
// 	else
//     get_current = reg_data * 50 + 100;

// 	pr_info("max77960 get charge current: reg(0x%02x),", reg_data);
// 	pr_info("max77960 get charge current: %dmA\n", get_current);
// 	return get_current;
// }

// static void max77960_charger_set_topoff_current(struct max77960_charger_data
// 	*chg_data, int cur, int time)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	u8 reg_data;

// 	if (cur >= 600)
// 		reg_data = 0x07;
// 	else if (cur >= 500)
// 		reg_data = 0x04;
// 	else if (cur >= 400)
// 		reg_data = 0x03;
// 	else if (cur >= 300)
// 		reg_data = 0x02;
// 	else if (cur >= 200)
// 		reg_data = 0x01;
// 	else
// 		reg_data = 0x00;

//   /* topoff timer setting*/
//   time = time / 10;
//   reg_data |= (time << 3);
// 	pr_info("max77960 set topoff cur: reg_data(0x%02x), topoff current(%d)\n",
// 		reg_data, cur);

// 	max77960_write_reg(regmap, MAX77960_CHG_CNFG_03, reg_data);
// }

// static int max77960_charger_get_charger_state(struct max77960_charger_data
// 	*chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	int state;
// 	u8 reg_data;

// 	max77960_read_reg(regmap, MAX77960_CHG_DTLS_01, &reg_data);
// 	reg_data = ((reg_data & BIT_CHG_DTLS) >> FFS(BIT_CHG_DTLS));
// 	pr_info("max77960 get charger state CHG_DTLS(0x%02x)\n", reg_data);

// 	switch (reg_data) {
// 		case 0x0:
// 		case 0x1:
// 		case 0x2:
// 			state = POWER_SUPPLY_STATUS_CHARGING;
// 			break;
// 		case 0x3:
// 		case 0x4:
// 			state = POWER_SUPPLY_STATUS_FULL;
// 			break;
// 		case 0x5:
// 		case 0x6:
// 		case 0x7:
// 			state = POWER_SUPPLY_STATUS_NOT_CHARGING;
// 			break;
// 		case 0x8:
// 		case 0xA:
// 		case 0xB:
// 			state = POWER_SUPPLY_STATUS_DISCHARGING;
// 			break;
// 		default:
// 			state = POWER_SUPPLY_STATUS_UNKNOWN;
// 			break;
// 	}

// 	return state;
// }

// static int max77960_charger_get_health_state(struct max77960_charger_data
// 	*chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;

// 	int state;
// 	u8  reg_data;

// 	max77960_read_reg(regmap, MAX77960_CHG_DTLS_01, &reg_data);
// 	reg_data = ((reg_data & BIT_BAT_DTLS) >> FFS(BIT_BAT_DTLS));

// 	pr_info("max77960 CHG_DTLS_01(0x%02x)\n",  reg_data);
// 	switch (reg_data) {
// 		case 0x00:
// 			pr_info("No battery and the charger is suspended\n");
// 			state = POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
// 			break;
// 		case 0x01:
// 			pr_info("battery is okay, but its voltage is low(<VPRECHG)\n");
// 			state = POWER_SUPPLY_HEALTH_GOOD;
// 			break;
// 		case 0x02:
// 			pr_info("battery dead\n");
// 			state = POWER_SUPPLY_HEALTH_DEAD;
// 			break;
// 		case 0x03:
// 			pr_info("battery good\n");
// 			state = POWER_SUPPLY_HEALTH_GOOD;
// 			break;
// 		case 0x04:
// 		pr_info("battery is okay but its voltage is low\n");
// 			state = POWER_SUPPLY_HEALTH_GOOD;
// 			break;
// 		case 0x05:
// 		pr_info("battery ovp\n");
// 			state = POWER_SUPPLY_HEALTH_OVERVOLTAGE;
// 			break;
// 		case 0x06:
// 		pr_info("battery ocp\n");
// 			state = POWER_SUPPLY_HEALTH_OVERVOLTAGE;
// 			break;
// 		case 0x07:
// 		pr_info("battery not available\n");
// 			state = POWER_SUPPLY_HEALTH_UNKNOWN;
// 			break;
// 		default:
// 		pr_info("battery unknown\n");
// 			state = POWER_SUPPLY_HEALTH_UNKNOWN;
// 			break;
// 	}

// #ifdef CONFIG_BATTERY_ENABLED
// 	if (state == POWER_SUPPLY_HEALTH_GOOD) {
// 		union power_supply_propval value;
// 		{
// 			struct power_supply *psy;
// 			int ret;
// 			int state_helth;

// 				psy = power_supply_get_by_name("battery");
// 			if (!psy)
// 				value.intval = 0;
// 			else {
// 				if (psy->desc->get_property != NULL) {
// 					state_helth = POWER_SUPPLY_PROP_HEALTH;
// 					ret = psy->desc->get_property(psy,
// 						state_helth, &value);
// 					if (ret < 0)
// 						value.intval = 0;
// 				}
// 				power_supply_put(psy);
// 			}
// 		}
// 	}
// #endif
// 	return state;
// }
// static int max77960_charger_get_property(struct power_supply *psy,
// 		enum power_supply_property psp,
// 		union power_supply_propval *val)
// {
// 	struct max77960_charger_data *chg_data = power_supply_get_drvdata(psy);
// 	u8 reg_data;

// 	switch (psp) {
// 		case POWER_SUPPLY_PROP_ONLINE:
// 			val->intval = POWER_SUPPLY_TYPE_BATTERY;
// 			if (max77960_read_reg(chg_data->regmap,
// 						MAX77960_CHG_INT_OK, &reg_data) == 0) {
// 			if (reg_data & BIT_CHGIN_OK)
// 					val->intval = POWER_SUPPLY_TYPE_MAINS;
// 				}
// 			break;

// 		case POWER_SUPPLY_PROP_STATUS:
// 			val->intval = max77960_charger_get_charger_state(chg_data);
// 			break;

// 		case POWER_SUPPLY_PROP_HEALTH:
// 			val->intval = max77960_charger_get_health_state(chg_data);
// 			break;

// 		case POWER_SUPPLY_PROP_CURRENT_NOW:
// 			val->intval = max77960_charger_get_charge_current(chg_data);
// 			break;

//         case POWER_SUPPLY_PROP_CURRENT_MAX:
//             val->intval = max77960_charger_get_input_current(chg_data);
// 		break;

// 		case POWER_SUPPLY_PROP_CHARGE_TYPE:
// 			if (!chg_data->is_charging)
// 				val->intval = POWER_SUPPLY_CHARGE_TYPE_NONE;
// 			else
// 				val->intval = POWER_SUPPLY_CHARGE_TYPE_FAST;
// 		break;
// 		default:
// 			return -EINVAL;
// 	}

// 	return 0;
// }

// static int max77960_charger_set_property(struct power_supply *psy,
// 		enum power_supply_property psp,
// 		const union power_supply_propval *val)
// {
// 	struct max77960_charger_data *chg_data = power_supply_get_drvdata(psy);

// 	switch (psp) {
// 		case POWER_SUPPLY_PROP_STATUS:
// 			chg_data->status = val->intval;
// 			break;
// 			/* val->intval : type */
// 		case POWER_SUPPLY_PROP_ONLINE:
// 			/* check and unlock */
// 			max77960_charger_check_unlock_state(chg_data);
// 			max77960_charger_set_state(chg_data, val->intval);
//            /* apply charge current */
//             max77960_charger_set_input_current(chg_data,
//                     chg_data->fast_charge_current);  /* mA */
// 		break;
// 		case POWER_SUPPLY_PROP_CURRENT_NOW:
// 			max77960_charger_set_charge_current(chg_data,
// 				val->intval);	/* mA */
//             chg_data->fast_charge_current = val->intval;	/* mA */
// 			break;
// 		case POWER_SUPPLY_PROP_CURRENT_MAX:
// 			max77960_charger_set_input_current(chg_data,
// 				val->intval);	/* mA */
// 		chg_data->input_current_limit = val->intval;	/* mA */
// 		break;
// 		default:
// 			return -EINVAL;
// 	}

// 	return 0;
// }

// static void max77960_charger_initialize(struct max77960_charger_data *chg_data)
// {
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data;

// 	/*
// 	 * interrupt mask - if you want to enable some bits,
// 	 * you should clear them
// 	 */
// 	reg_data = BIT_AICL_M | BIT_CHGIN_M | BIT_B2SOVRC_M
// 		| BIT_CHG_M | BIT_BAT_M | BIT_CHGINILIM_M
// 		| BIT_DISQBAT_M | BIT_OTG_PLIM_M;
//     max77960_write_reg(regmap, MAX77960_CHG_INT_MASK, reg_data);

// 	/* unlock charger protect */
// 	reg_data = (0x03 << 2);
// 	max77960_write_reg(regmap, MAX77960_CHG_CNFG_06, reg_data);

//     /* charge current (mA) */
// 	max77960_charger_set_charge_current(chg_data,
// 		chg_data->fast_charge_current);

//   	/* input current limit (mA) */
// 	max77960_charger_set_input_current(chg_data,
// 		chg_data->input_current_limit);

//     /* topoff current(mA) and topoff timer(min) */
// 	max77960_charger_set_topoff_current(chg_data,
// 		chg_data->topoff_current, chg_data->topoff_timer);

// 	/*
// 	 * Smart Power Selector
// 	 * CHG on, OTG off, DCDC on
// 	 */
//     reg_data = (0x05 << 0);
//     max77960_write_reg(regmap, MAX77960_CHG_CNFG_00, reg_data);

// 	/*
//      * fast charge timer, enable STAT output
//      * restart threshold, prequal charge enable
// 	 */
//     reg_data = BIT_STAT_EN | BIT_PQEN | (0x01 << 4) | (0x01 << 0);
//     max77960_write_reg(regmap, MAX77960_CHG_CNFG_01, reg_data);

// 	/*
//      * charge termination voltage (8V)
// 	 */
//     reg_data = (0x00 << 0);
//     max77960_write_reg(regmap, MAX77960_CHG_CNFG_04, reg_data);
// }

// static void max77960_chgin_isr_work(struct work_struct *work)
// {
// 	struct max77960_charger_data *chg_data =
// 		container_of(work, struct max77960_charger_data, chgin_work);
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data;
//     bool chg_input;

// 	max77960_read_reg(regmap, MAX77960_CHG_INT_MASK, &reg_data);
// 	reg_data |= BIT_CHGIN_M;
// 	max77960_write_reg(regmap, MAX77960_CHG_INT_MASK, reg_data);

// 	max77960_read_reg(regmap, MAX77960_CHG_DTLS_00, &reg_data);
//     chg_data->details_0 = reg_data;
// 	max77960_read_reg(regmap, MAX77960_CHG_DTLS_01, &reg_data);
// 	chg_data->details_1 = reg_data;
// 	max77960_read_reg(regmap, MAX77960_CHG_DTLS_02, &reg_data);
//     chg_data->details_2 = reg_data;

//     chg_input = max77818_charger_present_input(chg_data);
// 	if (chg_input) {
//         max77960_charger_set_state(chg_data, true);
//         /* apply charge current */
//         max77960_charger_set_input_current(chg_data,
// 				chg_data->fast_charge_current);  /* mA */
// 	} else {
//         max77960_charger_set_state(chg_data, false);
//     }
// 	pr_info("max77960 CHG_INT_CHGIN: charge input %s\n",
// 		chg_input ? "inserted" : "removed");
// 	max77960_read_reg(regmap, MAX77960_CHG_INT_MASK, &reg_data);
// 	reg_data &= ~BIT_CHGIN_M;
// 	max77960_write_reg(regmap, MAX77960_CHG_INT_MASK, reg_data);
// }

// static irqreturn_t max77960_charger_irq(int irq, void *data)
// {
// 	struct max77960_charger_data *chg_data = data;
// 	struct regmap *regmap = chg_data->regmap;
// 	u8 reg_data;

// 	max77960_read_reg(regmap, MAX77960_CHG_INT, &reg_data);

// 	if (reg_data & BIT_CHGIN_I)
//         queue_work(chg_data->wqueue, &chg_data->chgin_work);
// 	/*
// 	 * add other interrupts
// 	 * else {
// 	 *
// 	 *	}
// 	 */
// 	return IRQ_HANDLED;
// }