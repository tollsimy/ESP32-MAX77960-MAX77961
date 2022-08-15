#include "ESP32_MAX77960.h"
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

static const char* TAG="ESP32_MAX77960";

static const char* ALREADY_INIT="Already initialized";
static const char* NOT_INIT="Not initialized";
static const char* NOT_CONF="Not configured";
static const char* NOT_CHG_CONF="Charge not configured";

/* Definitions of the Black Magic strings */
#define C(k, v) [v] = #k, 
const char * const MAX77960_CHGIN_DTLS_name[] = { CHGIN_DTLS_ENUM };
const char * const MAX77960_OTG_DTLS_name[] = { OTG_DTLS_ENUM };
const char * const MAX77960_BAT_DTLS_name[] = { BAT_DTLS_ENUM };
const char * const MAX77960_CHG_DTLS_name[] = { CHG_DTLS_ENUM };
const char * const MAX77960_THM_DTLS_name[] = { THM_DTLS_ENUM };
const char * const MAX77960_CHG_MODE_name[] = { CHG_MODE_ENUM };
#undef C

// ---------------- I2C AUXILIARY FUNCTIONS ----------------

/**
 *  @brief  write an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
static void write8(ESP32_MAX77960* MAX, uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX->i2c_port, MAX77960_I2C_ADDR, buffer, 2, 1000 / portTICK_PERIOD_MS));
}

/**
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
static uint8_t read8(ESP32_MAX77960* MAX, uint8_t reg) {
    uint8_t buffer[1] = {reg};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX->i2c_port, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(MAX->i2c_port, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    return buffer[0];
}

/**
 *  @brief  Reads <SIZE> bytes over I2C
 *  @param  reg
 *  @param  data pointer to the data buffer
 *  @param  size number of bytes to read
 *  @return value
 */
static void read_data(ESP32_MAX77960* MAX, uint8_t reg, uint8_t *data, size_t size) {
    uint8_t buffer[1] = {reg};
    ESP_ERROR_CHECK(i2c_master_write_to_device(MAX->i2c_port, MAX77960_I2C_ADDR, buffer, 1, 1000 / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_read_from_device(MAX->i2c_port, MAX77960_I2C_ADDR, data, size, 1000 / portTICK_PERIOD_MS));
}

/**
 * @brief Write bits in a register
 * 
 * @param reg register to write to
 * @param mask mask to apply
 * @param value value to write
 */
static void writeBits(ESP32_MAX77960* MAX, uint8_t reg, uint8_t mask, uint8_t value) {
    uint8_t tmp = read8(MAX, reg);
    tmp &= ~mask;
    tmp |= (value & mask);
    write8(MAX, reg, tmp);
}

// ---------------- PERIPHERALS GENERAL INFO GETTERS ----------------

static void max77960_get_number_of_cell(ESP32_MAX77960* MAX){
    assert(MAX->_is_init==true && NOT_INIT);
    uint8_t value = read8(MAX, CHG_DTLS_02_REG);
    MAX->NUM_CELL = (value & NUM_CELL_DTLS_MASK);
}

static void max77960_get_switching_frequency(ESP32_MAX77960* MAX){
    assert(MAX->_is_init==true && NOT_INIT);
    uint8_t value = read8(MAX, CHG_DTLS_02_REG);
    MAX->FSW = (value & FSW_DTLS_MASK) >> FSW_DTLS;
}

// ---------------- INITIALIZATION FUNCTIONS ----------------

/**
 *  @brief  Initialize the MAX77960 peripheral
 */
void MAX_init(ESP32_MAX77960* MAX, uint8_t i2c_port) {

    assert(MAX->_is_init!=true && ALREADY_INIT);
    MAX->i2c_port = i2c_port;

    MAX->_is_init=true;

    max77960_get_number_of_cell(MAX);
    max77960_get_switching_frequency(MAX);
    ESP_LOGI(TAG, "MAX77960 found: %d cells max, %d KHz switching frequency", MAX->NUM_CELL ? 3 : 2, MAX->FSW ? 600 : 1200);
    if(MAX->NUM_CELL==1){
        ESP_LOGI(TAG, "MAX77960 is set in 3S mode");
    }
    else{
        ESP_LOGI(TAG, "MAX77960 is set in 2S mode");
    }
}

MAX77960_DETAILS_STRINGS_t MAX_get_details(ESP32_MAX77960* MAX){

    assert(MAX->_is_init==true && NOT_INIT);
    uint8_t buf[4];
    MAX77960_DETAILS_STRINGS_t details_strings={0};

    read_data(MAX, CHG_DTLS_00_REG, buf, 4);

    MAX->details.CHGIN_det=(buf[0] & CHGIN_DTLS_MASK) >> CHGIN_DTLS;
    MAX->details.OTG_det=(buf[0] & OTG_DTLS_MASK) >> OTG_DTLS;
    MAX->details.QB_enabled=(buf[0] & QB_DTLS_MASK) >> QB_DTLS;
    MAX->details.BAT_det=(buf[1] & BAT_DTLS_MASK) >> BAT_DTLS;
    MAX->details.CHG_det=(buf[1] & CHG_DTLS_MASK) >> CHG_DTLS;
    MAX->details.THM_det=(buf[2] & THM_DTLS_MASK) >> THM_DTLS;
    MAX->details.CHG_enabled=(buf[2] & APP_MODE_DTLS_MASK) >> APP_MODE_DTLS;
    MAX->details.mode=(buf[3] & MODE_MASK) >> MODE;

    details_strings.CHGIN_det_s=MAX77960_CHGIN_DTLS_name[MAX->details.CHGIN_det];
    details_strings.OTG_det_s=MAX77960_OTG_DTLS_name[MAX->details.OTG_det];
    details_strings.QB_enabled_s=MAX->details.QB_enabled ? "On" : "Off";
    details_strings.BAT_det_s=MAX77960_BAT_DTLS_name[MAX->details.BAT_det];
    details_strings.CHG_det_s=MAX77960_CHG_DTLS_name[MAX->details.CHG_det];
    details_strings.THM_det_s=MAX77960_THM_DTLS_name[MAX->details.THM_det];
    details_strings.CHG_enabled_s=MAX->details.CHG_enabled ? "Enabled" : "Disabled";
    details_strings.mode_s=MAX77960_CHG_MODE_name[MAX->details.mode];
    return details_strings;
}

void MAX_set_chg_conf(ESP32_MAX77960* MAX, MAX77960_CHG_CONFIG_t chg_conf){
    assert(MAX->_is_init==true && NOT_INIT);
    //set INLIM
    writeBits(MAX, CHG_CNFG_08_REG, CHGIN_ILIM_MASK, chg_conf.INLIM << CHGIN_ILIM);
    //set INLIM_CLK and OTG_INLIM
    writeBits(MAX, CHG_CNFG_09_REG, INLIM_CLK_MASK || OTG_ILIM_MASK, 
                (chg_conf.INLIM_CLK_ << INLIM_CLK) || (chg_conf.OTG_ILIM_ << OTG_ILIM));
    //set ISET
    writeBits(MAX, CHG_CNFG_02_REG, CHGCC_MASK, chg_conf.ISET << CHGCC);
    //set ITO and Top Off timer
    writeBits(MAX, CHG_CNFG_03_REG, TO_ITH_MASK || TO_TIME_MASK, 
                (chg_conf.ITO << TO_ITH) || (chg_conf.TO_timer << TO_TIME));
    //set VSET
    if(MAX->NUM_CELL==1){
        writeBits(MAX, CHG_CNFG_04_REG, CHG_CV_PRM_MASK, chg_conf.VSET_3S << CHG_CV_PRM);
    }
    else{
        writeBits(MAX, CHG_CNFG_04_REG, CHG_CV_PRM_MASK, chg_conf.VSET_2S << CHG_CV_PRM);
    }
    //set ITRICKLE
    writeBits(MAX, CHG_CNFG_05_REG, ITRICKLE_MASK, chg_conf.I_TRICKLE << ITRICKLE);
    //set Fast charge timer and CHG_RST
    writeBits(MAX, CHG_CNFG_01_REG, FCHGTIME_MASK || CHG_RSTRT_MASK, 
                (chg_conf.FC_timer << FCHGTIME) || (chg_conf.CHG_RST << CHG_RSTRT));
    //set JEITA 
    writeBits(MAX, CHG_CNFG_07_REG, JEITA_EN_MASK, chg_conf.JEITA_enabled << JEITA_EN);

    MAX->_is_charge_configured=true;
}

void MAX_set_conf(ESP32_MAX77960* MAX, MAX77960_CONFIG_t conf){
    assert(MAX->_is_init==true && NOT_INIT);
    //set B2SOVRC
    writeBits(MAX, CHG_CNFG_05_REG, B2SOVRC_MASK, conf.B2SOVRC_ << B2SOVRC);
    //set MINVSYS
    if(MAX->NUM_CELL==1){
        writeBits(MAX, CHG_CNFG_09_REG, MINVSYS_MASK, conf.MINVSYS_3S << MINVSYS);
    }
    else{
        writeBits(MAX, CHG_CNFG_09_REG, MINVSYS_MASK, conf.MINVSYS_2S << MINVSYS);
    }
    //set REGTEMP
    writeBits(MAX, CHG_CNFG_07_REG, REGTEMP_MASK, conf.REGTEMP_ << REGTEMP);
    //set VCHGIN_REG
    writeBits(MAX, CHG_CNFG_10_REG, VCHGIN_REG_MASK, conf.VCHGIN_REG_ << VCHGIN_REG);

    MAX->_is_configured=true;
}

/**
 * @brief Set I2C mode enabled and start charge with value programmed by 
 * registers, can't be disabled, must restart the MAX77960
 * 
 * @param MAX Pointer to MAX77960 structure
 */
void MAX_set_chg_i2c_mode(ESP32_MAX77960 * MAX){
    assert(MAX->_is_init==true && NOT_INIT);
    assert(MAX->_is_charge_configured==true && NOT_CHG_CONF);
    assert(MAX->_is_configured==true && NOT_CONF);

    writeBits(MAX, CHG_CNFG_00_REG, COMM_MODE_MASK, 0x01 << COMM_MODE);
}

//TODO: implements AICL
//TODO: interrupts?
//TODO: unlock charge protection only if it has been configured


/**
 *	@brief Unlock charger protection, return true if successful
 */
bool MAX_charger_protection_unlock(ESP32_MAX77960* MAX)
{   
    assert(MAX->_is_init==true && NOT_INIT);

    const uint8_t MAX_TRY = 10;
    uint8_t ntry = 0;
    uint8_t reg_data = 0;

    while (1) {
        writeBits(MAX, CHG_CNFG_06_REG, CHGPROT_MASK, MAX77960_CHGPROT_UNLOCKED << CHGPROT);
        reg_data=read8(MAX, CHG_CNFG_06_REG);
        MAX->chgprot_state = (reg_data & CHGPROT_MASK) >> CHGPROT;
        if (MAX->chgprot_state == MAX77960_CHGPROT_UNLOCKED)
            return true;
        else if (++ntry > MAX_TRY)
            return false;
    }
}

/**
 *	@brief Lock charger protection, return true if successful
 */
bool MAX_charger_protection_lock(ESP32_MAX77960* MAX)
{   
    assert(MAX->_is_init==true && NOT_INIT);
    const uint8_t MAX_TRY = 10;
    uint8_t ntry = 0;
    uint8_t reg_data = 0;

    while (1) {
        writeBits(MAX, CHG_CNFG_06_REG, CHGPROT_MASK, MAX77960_CHGPROT_LOCKED_0 << CHGPROT);
        reg_data=read8(MAX, CHG_CNFG_06_REG);
        MAX->chgprot_state = (reg_data & CHGPROT_MASK) >> CHGPROT;
        if (MAX->chgprot_state == MAX77960_CHGPROT_LOCKED_0)
            return true;
        else if (++ntry > MAX_TRY)
            return false;
    }
}

void MAX_set_mode(ESP32_MAX77960* MAX, MAX77960_CHG_MODE_t mode){
    
    assert(MAX->_is_init==true && NOT_INIT);
    assert(MAX->_is_charge_configured==true && NOT_CHG_CONF);
    assert(MAX->_is_configured==true && NOT_CONF);

    writeBits(MAX, CHG_CNFG_00_REG, MODE_MASK, mode << MODE);
}

/*
 *	Perform software reset to type O registers.
 */
void MAX_software_reset(ESP32_MAX77960* MAX){   
    assert(MAX->_is_init==true && NOT_INIT);
    write8(MAX, SWRST_REG, SWRST_COMMAND << SWRST);
    //TODO: is this right? Does reset also lock charger protection?
    MAX->_is_charge_configured=false;
    MAX->_is_configured=false;
}

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