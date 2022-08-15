#ifndef __ESP32_MAX77960_H__
#define __ESP32_MAX77960_H__

#include <stdint.h>
#include "driver/i2c.h"
#include <esp_err.h>


#define MAX77960_I2C_ADDR   0x69

/****************************************************************************
 *                    MAX77960/MAX77961 Register Map                        *
 ****************************************************************************/

/* CID */
#define	CID_REG				0x00
#define REVISION			(5)
#define REVISION_MASK		0x07 << REVISION
#define VERSION				(0)
#define VERSION_MASK		0x1F << VERSION

/* SWREST */
#define	SWRST_REG			0x01
#define SWRST				(0)
#define SWRST_MASK          0xFF << SWRST
#define SWRST_COMMAND		0xA5

/* TOP_INT */
#define	TOP_INT_REG			0x02
#define TSHDN_I				(2)
#define TSHDN_I_MASK		0x01 << TSHDN_I
#define SYSOVLO_I 			(1)
#define SYSOVLO_I_MASK		0x01 << SYSOVLO_I
#define SYSUVLO_I			(0)
#define SYSUVLO_I_MASK		0x01 << SYSUVLO_I

/* TOP_INT_MASK */
#define	TOP_INT_MASK_REG	0x03
#define TSHDN_M				(2)
#define TSHDN_M_MASK		0x01 << TSHDN_M
#define SYSOVLO_M			(1)
#define SYSOVLO_M_MASK		0x01 << SYSOVLO_M
#define SYSUVLO_M			(0)
#define SYSUVLO_M_MASK		0x01 << SYSUVLO_M

/* TOP_INT_OK */
#define	TOP_INT_OK_REG		0x04
#define TSHDN_OK			(2)
#define TSHDN_OK_MASK		0x01 << TSHDN_OK
#define SYSOVLO_OK			(1)
#define SYSOVLO_OK_MASK		0x01 << SYSOVLO_OK
#define SYSUVLO_OK			(0)
#define SYSUVLO_OK_MASK		0x01 << SYSUVLO_OK

/* CHG_INT */
#define	CHG_INT_REG			0x10
#define	AICL_I              (7)
#define	AICL_I_MASK         0x01 << AICL_I
#define	CHGIN_I             (6)
#define	CHGIN_I_MASK        0x01 << CHGIN_I
#define	B2SOVRC_I           (5)
#define	B2SOVRC_I_MASK      0x01 << B2SOVRC_I
#define	CHG_I               (4)
#define	CHG_I_MASK          0x01 << CHG_I
#define	BAT_I               (3)
#define	BAT_I_MASK          0x01 << BAT_I
#define	CHGINILIM_I         (2)
#define	CHGINILIM_I_MASK    0x01 << CHGINILIM_I
#define	DISQBAT_I           (1)
#define	DISQBAT_I_MASK      0x01 << DISQBAT_I
#define	OTG_PLIM_I          (0)
#define	OTG_PLIM_I_MASK     0x01 << OTG_PLIM_I

/* CHG_INT_MASK */
#define	CHG_INT_MASK_REG	0x11
#define	AICL_M              (7)
#define	AICL_M_MASK         0x01 << AICL_M
#define	CHGIN_M             (6)
#define	CHGIN_M_MASK        0x01 << CHGIN_M
#define	B2SOVRC_M           (5)
#define	B2SOVRC_M_MASK      0x01 << B2SOVRC_M
#define	CHG_M               (4)
#define	CHG_M_MASK          0x01 << CHG_M
#define	BAT_M               (3)
#define	BAT_M_MASK          0x01 << BAT_M
#define	CHGINILIM_M         (2)
#define	CHGINILIM_M_MASK    0x01 << CHGINILIM_M
#define	DISQBAT_M           (1)
#define	DISQBAT_M_MASK      0x01 << DISQBAT_M
#define	OTG_PLIM_M          (0)
#define	OTG_PLIM_M_MASK     0x01 << OTG_PLIM_M

/* CHG_INT_OK */
#define	CHG_INT_OK_REG		0x12
#define	AICL_OK             (7)
#define	AICL_OK_MASK        0x01 << AICL_OK
#define	CHGIN_OK            (6)
#define	CHGIN_OK_MASK       0x01 << CHGIN_OK
#define	B2SOVRC_OK          (5)
#define	B2SOVRC_OK_MASK     0x01 << B2SOVRC_OK
#define	CHG_OK              (4)
#define	CHG_OK_MASK         0x01 << CHG_OK
#define	BAT_OK              (3)
#define	BAT_OK_MASK         0x01 << BAT_OK
#define	CHGINILIM_OK        (2)
#define	CHGINILIM_OK_MASK   0x01 << CHGINILIM_OK
#define	DISQBAT_OK          (1)
#define	DISQBAT_OK_MASK     0x01 << DISQBAT_OK
#define	OTG_PLIM_OK         (0)
#define	OTG_PLIM_OK_MASK    0x01 << OTG_PLIM_OK

/* Charger details 00 */
#define CHG_DTLS_00_REG		0x13
#define CHGIN_DTLS			(5)
#define CHGIN_DTLS_MASK		0x03 << CHGIN_DTLS
#define OTG_DTLS			(3)
#define OTG_DTLS_MASK		0x03 << OTG_DTLS
#define QB_DTLS				(0)
#define QB_DTLS_MASK		0x01 << QB_DTLS

/* Charger details 01 */
#define CHG_DTLS_01_REG  	0x14
#define TREG				(7)
#define TREG_MASK			0x01 << TREG
#define BAT_DTLS			(4)
#define BAT_DTLS_MASK		0x07 << BAT_DTLS
#define CHG_DTLS			(0)
#define CHG_DTLS_MASK		0x0F << CHG_DTLS

/* Charger details 02 */
#define CHG_DTLS_02_REG		0x15
#define	THM_DTLS			(4)
#define THM_DTLS_MASK		0x07 << THM_DTLS
#define APP_MODE_DTLS 		(3)
#define APP_MODE_DTLS_MASK 	0x01 << APP_MODE_DTLS
#define FSW_DTLS			(1)
#define FSW_DTLS_MASK		0x03 << FSW_DTLS
#define NUM_CELL_DTLS 		(0)
#define NUM_CELL_DTLS_MASK 	0x01 << NUM_CELL_DTLS

/* Charger configuration 00 */
#define	CHG_CNFG_00_REG 	0x16
#define COMM_MODE			(7)
#define COMM_MODE_MASK		1 << COMM_MODE
#define DISIBS				(6)
#define DISIBS_MASK			1 << DISIBS
#define STBY_EN				(5)
#define STBY_EN_MASK		1 << STBY_EN
#define WDTEN				(4)
#define WDTEN_MASK			1 << WDTEN
#define MODE				(0)
#define MODE_MASK			0x0F << MODE

/* Charger configuration 01 */
#define	CHG_CNFG_01_REG		0x17
#define PQNE				(7)
#define PQNE_MASK			0x01 << PQNE
#define LPM					(6)
#define LPM_MASK			0x01 << LPM
#define CHG_RSTRT			(4)
#define CHG_RSTRT_MASK		0x03 << CHG_RSTRT
#define STAT_EN 			(3)
#define STAT_EN_MASK		0x01 << STAT_EN
#define FCHGTIME			(0)
#define FCHGTIME_MASK		0x07 << FCHGTIME

/* Charger configuration 02 */
#define	CHG_CNFG_02_REG		0x18
#define CHGCC				(0)
#define CHGCC_MASK			0x3F << CHGCC

/* Charger configuration 03 */
#define	CHG_CNFG_03_REG		0x19
#define SYS_TRACK_DIS		(7)
#define SYS_TRACK_DIS_MASK 	0x01 << SYS_TRACK_DIS
#define B2SOVRC_DTC			(6)
#define B2SOVRC_DTC_MASK    0x01 << B2SOVRC_DTC
#define TO_TIME				(3)
#define TO_TIME_MASK   		0x07 << TO_TIME
#define TO_ITH				(0)
#define TO_ITH_MASK			0x07 << TO_ITH

/* Charger configuration 04 */
#define	CHG_CNFG_04_REG		0x1A
#define CHG_CV_PRM			(0)
#define CHG_CV_PRM_MASK		0x3F << CHGCC

/* Charger configuration 05 */
#define	CHG_CNFG_05_REG		0x1B
#define ITRICKLE			(4)
#define ITRICKLE_MASK		0x03 << ITRICKLE
#define B2SOVRC				(0)
#define B2SOVRC_MASK		0x0F << B2SOVRC

/* Charger configuration 06 */
#define	CHG_CNFG_06_REG		0x1C
#define CHGPROT				(2)
#define CHGPROT_MASK		0x03 << CHGPROT
#define WDTCLR				(0)
#define WDTCLR_MASK			0x03 << WDTCLR
#define WDTCLR_CLEAR		0x03	    // Writing 0x03 to WDTCLR clears the
										// watchdog timer when it is enabled

/* Charger configuration 07 */
#define	CHG_CNFG_07_REG		0x1D
#define JEITA_EN			(7)
#define JEITA_EN_MASK		0x01 << JEITA_EN
#define REGTEMP				(3)
#define REGTEMP_MASK		0x0F << REGTEMP
#define VCHGCV_COOL 		(2)
#define VCHGCV_COOL_MASK	0x01 << VCHGCV_COOL
#define ICHGCC_COOL			(1)
#define ICHGCC_COOL_MASK	0x01 << ICHGCC_COOL
#define FSHIP_MODE 			(0)
#define FSHIP_MODE_MASK		0x01 << FSHIP_MODE

/* Charger configuration 08 */
#define	CHG_CNFG_08_REG		0x1E
#define CHGIN_ILIM			(0)
#define CHGIN_ILIM_MASK		0x7F << CHGIN_ILIM

/* Charger configuration 09 */
#define	CHG_CNFG_09_REG		0x1F
#define INLIM_CLK			(6)
#define INLIM_CLK_MASK		0x03 << INLIM_CLK
#define OTG_ILIM			(3)
#define OTG_ILIM_MASK		0x07 << OTG_ILIM
#define MINVSYS				(0)
#define MINVSYS_MASK		0x07 << MINVSYS

/* Charger configuration 10 */
#define	CHG_CNFG_10_REG		0x20
#define VCHGIN_REG			(1)
#define VCHGIN_REG_MASK		0x1F << VCHGIN_REG
#define DISKIP				(0)
#define DISKIP_MASK			0x01 << DISKIP


/*
 * Charger details 0
 */
/* Black Magic to obtain the enum and the strings to print */
/* NOTE: this macros works only if enum index greater or equal to 0 */

/* CHGIN DTLS */
#define CHGIN_DTLS_ENUM C(MAX77960_CHGIN_DTLS_LESS_UVLO, 0x0)\
                        C(MAX77960_CHGIN_DTLS_MORE_OVLO, 0x2)\
                        C(MAX77960_CHGIN_DTLS_VALID, 0x3)
#define C(k, v) k = v,
typedef enum { CHGIN_DTLS_ENUM } MAX77960_CHGIN_DTLS_t;
#undef C   
extern const char * const MAX77960_CHGIN_DTLS_name[];
/* End of CHGIN DTLS */

/* OTG DTLS */
#define OTG_DTLS_ENUM C(MAX77960_OTG_DTLS_LESS_UVLO, 0x0)\
                        C(MAX77960_OTG_DTLS_CURR_LIMIT, 0x1)\
                        C(MAX77960_OTG_DTLS_MORE_OVLO, 0x2)\
                        C(MAX77960_OTG_DTLS_DISABLED_OR_VALID, 0x3)
#define C(k, v) k = v,
typedef enum { OTG_DTLS_ENUM } MAX77960_OTG_DTLS_t;
#undef C   
extern const char * const MAX77960_OTG_DTLS_name[];
/* End of OTG DTLS */

/*
 * Charger details 1
 */

/* BAT DTLS */
#define BAT_DTLS_ENUM C(MAX77960_BAT_DTLS_BAT_REMOVED, 0x0)\
                        C(MAX77960_BAT_DTLS_BAT_PRECHARGE, 0x1)\
                        C(MAX77960_BAT_DTLS_BAT_TOO_TIME, 0x2)\
                        C(MAX77960_BAT_DTLS_BAT_OK, 0x3)\
                        C(MAX77960_BAT_DTLS_BAT_LOW, 0x4)\
                        C(MAX77960_BAT_DTLS_BAT_OVERVOLT, 0x5)\
                        C(MAX77960_BAT_DTLS_BAT_OVERCURR, 0x6)\
                        C(MAX77960_BAT_DTLS_BAT_UNKNOWN, 0x7)
#define C(k, v) k = v,
typedef enum { BAT_DTLS_ENUM } MAX77960_BAT_DTLS_t;
#undef C    
extern const char * const MAX77960_BAT_DTLS_name[];
/* End of BAT DTLS */

/* CHG DTLS */
#define CHG_DTLS_ENUM C(MAX77960_CHG_DTLS_PRE_TRICKLE, 0x0)\
                        C(MAX77960_CHG_DTLS_FAST_CHARGE_CC, 0x1)\
                        C(MAX77960_CHG_DTLS_FAST_CHARGE_CV, 0x2)\
                        C(MAX77960_CHG_DTLS_TOP_OFF, 0x3)\
                        C(MAX77960_CHG_DTLS_DONE, 0x4)\
                        C(MAX77960_CHG_DTLS_OFF_OR_INVALID, 0x5)\
                        C(MAX77960_CHG_DTLS_TIMER_FAULT, 0x6)\
                        C(MAX77960_CHG_DTLS_QBAT_DISABLED, 0x7)\
                        C(MAX77960_CHG_DTLS_IN_INVALID_CHG_DISABLED, 0x8)\
                        C(MAX77960_CHG_DTLS_OVERTEMP, 0xA)\
                        C(MAX77960_CHG_DTLS_WDT_EXPIRED, 0xB)\
                        C(MAX77960_CHG_DTLS_JEITA_CONDITION, 0xC)\
                        C(MAX77960_CHG_DTLS_BAT_REMOVED, 0xD)
#define C(k, v) k = v,
typedef enum { CHG_DTLS_ENUM } MAX77960_CHG_DTLS_t;
#undef C
extern const char * const MAX77960_CHG_DTLS_name[];
/* End of CHG DTLS */

/*
 * Charger details 2
 */

/* THM DTLS */
#define THM_DTLS_ENUM C(MAX77960_THM_DTLS_TOO_LOW, 0x0)\
                        C(MAX77960_THM_DTLS_LOW, 0x1)\
                        C(MAX77960_THM_DTLS_NORMAL, 0x2)\
                        C(MAX77960_THM_DTLS_WARM, 0x3)\
                        C(MAX77960_THM_DTLS_TOO_HOT, 0x4)\
                        C(MAX77960_THM_DTLS_REMOVED, 0x5)\
                        C(MAX77960_THM_DTLS_THM_DISABLED, 0x6)
#define C(k, v) k = v,
typedef enum { THM_DTLS_ENUM } MAX77960_THM_DTLS_t;
#undef C   
extern const char * const MAX77960_THM_DTLS_name[];
/* End of THM DTLS */

typedef enum {
	MAX77960_FSW_DTLS_600_KHZ       = 0x0,
    MAX77960_FSW_DTLS_1200_KHZ      = 0x1
} MAX77960_FSW_DTLS_t;

typedef enum {
	MAX77960_NUM_CELL_DTLS_2S       = 0x0,
    MAX77960_NUM_CELL_DTLS_3S       = 0x1
} MAX77960_NUM_CELL_DTLS_t;

/*
 * Charger configuration 0
 */

/* CHG MODE */
#define CHG_MODE_ENUM C(MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_OFF_0, 0x0)\
                        C(MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_OFF_1, 0x1)\
                        C(MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_OFF_2, 0x2)\
                        C(MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_OFF_3, 0x3)\
                        C(MAX77960_MODE_CHG_OFF_OTG_OFF_DCDC_ON, 0x4)\
                        C(MAX77960_MODE_CHG_ON_OTG_OFF_DCDC_ON_0, 0x5)\
                        C(MAX77960_MODE_CHG_ON_OTG_OFF_DCDC_ON_1, 0x6)\
                        C(MAX77960_MODE_CHG_ON_OTG_OFF_DCDC_ON_2, 0x7)\
                        C(MAX77960_MODE_CHG_OFF_OTG_ON_DCDC_OFF, 0xA)
#define C(k, v) k = v,
typedef enum { CHG_MODE_ENUM } MAX77960_CHG_MODE_t;
#undef C
extern const char * const MAX77960_CHG_MODE_name[];

/* End of CHG MODE */

/*
 * Charger configuration 1
 */

typedef enum {
	MAX77960_CHG_RSTRT_100MV	= 0x00,
	MAX77960_CHG_RSTRT_150MV	= 0x10,
	MAX77960_CHG_RSTRT_200MV	= 0x20,
	MAX77960_CHG_RSTRT_DISABLE	= 0x30
} MAX77960_CHG_RSTRT_t;

typedef enum {
	MAX77960_FCHGTIME_DISABLE	= 0x00,
	MAX77960_FCHGTIME_3HRS,
	MAX77960_FCHGTIME_4HRS,
	MAX77960_FCHGTIME_5HRS,
	MAX77960_FCHGTIME_6HRS,
	MAX77960_FCHGTIME_7HRS,
	MAX77960_FCHGTIME_8HRS,
	MAX77960_FCHGTIME_10HRS
} MAX77960_FCHGTIME_t;

/*
 * Charger configuration 2
 */

typedef enum {
	MAX77960_CHG_CHGCC_100MA    = 0x00,
    MAX77960_CHG_CHGCC_150MA    = 0x01,
    MAX77960_CHG_CHGCC_200MA    = 0x02,
    MAX77960_CHG_CHGCC_250MA    = 0x03,
    MAX77960_CHG_CHGCC_300MA    = 0x04,
    MAX77960_CHG_CHGCC_350MA    = 0x05,
    MAX77960_CHG_CHGCC_400MA    = 0x06,
    MAX77960_CHG_CHGCC_450MA    = 0x07,
    MAX77960_CHG_CHGCC_500MA    = 0x08,
    MAX77960_CHG_CHGCC_600MA    = 0x09,
    MAX77960_CHG_CHGCC_700MA    = 0x0A,
    MAX77960_CHG_CHGCC_800MA    = 0x0B,
    MAX77960_CHG_CHGCC_900MA    = 0x0C,
    MAX77960_CHG_CHGCC_1000MA   = 0x0D,
    MAX77960_CHG_CHGCC_1100MA   = 0x0E,
    MAX77960_CHG_CHGCC_1200MA   = 0x0F,
    MAX77960_CHG_CHGCC_1300MA   = 0x10,
    MAX77960_CHG_CHGCC_1400MA   = 0x11,
    MAX77960_CHG_CHGCC_1500MA   = 0x12,
    MAX77960_CHG_CHGCC_1600MA   = 0x13,
    MAX77960_CHG_CHGCC_1700MA   = 0x14,
    MAX77960_CHG_CHGCC_1800MA   = 0x15,
    MAX77960_CHG_CHGCC_1900MA   = 0x16,
    MAX77960_CHG_CHGCC_2000MA   = 0x17,
    MAX77960_CHG_CHGCC_2100MA   = 0x18,
    MAX77960_CHG_CHGCC_2200MA   = 0x19,
    MAX77960_CHG_CHGCC_2300MA   = 0x1A,
    MAX77960_CHG_CHGCC_2400MA   = 0x1B,
    MAX77960_CHG_CHGCC_2500MA   = 0x1C,
    MAX77960_CHG_CHGCC_2600MA   = 0x1D,
    MAX77960_CHG_CHGCC_2700MA   = 0x1E,
    MAX77960_CHG_CHGCC_2800MA   = 0x1F,
    MAX77960_CHG_CHGCC_2900MA   = 0x20,
    MAX77960_CHG_CHGCC_3000MA   = 0x21,
    MAX77960_CHG_CHGCC_3100MA   = 0x22,
    MAX77960_CHG_CHGCC_3200MA   = 0x23,
    MAX77960_CHG_CHGCC_3300MA   = 0x24,
    MAX77960_CHG_CHGCC_3400MA   = 0x25,
    MAX77960_CHG_CHGCC_3500MA   = 0x26,
    MAX77960_CHG_CHGCC_3600MA   = 0x27,
    MAX77960_CHG_CHGCC_3700MA   = 0x28,
    MAX77960_CHG_CHGCC_3800MA   = 0x29,
    MAX77960_CHG_CHGCC_3900MA   = 0x2A,
    MAX77960_CHG_CHGCC_4000MA   = 0x2B,
    MAX77960_CHG_CHGCC_4100MA   = 0x2C,
    MAX77960_CHG_CHGCC_4200MA   = 0x2D,
    MAX77960_CHG_CHGCC_4300MA   = 0x2E,
    MAX77960_CHG_CHGCC_4400MA   = 0x2F,
    MAX77960_CHG_CHGCC_4500MA   = 0x30,
    MAX77960_CHG_CHGCC_4600MA   = 0x31,
    MAX77960_CHG_CHGCC_4700MA   = 0x32,
    MAX77960_CHG_CHGCC_4800MA   = 0x33,
    MAX77960_CHG_CHGCC_4900MA   = 0x34,
    MAX77960_CHG_CHGCC_5000MA   = 0x35,
    MAX77960_CHG_CHGCC_5100MA   = 0x36,
    MAX77960_CHG_CHGCC_5200MA   = 0x37,
    MAX77960_CHG_CHGCC_5300MA   = 0x38,
    MAX77960_CHG_CHGCC_5400MA   = 0x39,
    MAX77960_CHG_CHGCC_5500MA   = 0x3A,
    MAX77960_CHG_CHGCC_5600MA   = 0x3B,
    MAX77960_CHG_CHGCC_5700MA   = 0x3C,
    MAX77960_CHG_CHGCC_5800MA   = 0x3D,
    MAX77960_CHG_CHGCC_5900MA   = 0x3E,
    MAX77960_CHG_CHGCC_6000MA   = 0x3F
} MAX77960_CHG_CHGCC_t;

/*
 * Charger configuration 3
 */

typedef enum {
	MAX77960_B2SOVRC_DTC_6MS	= 0x0,
	MAX77960_B2SOVRC_DTC_100MS	= 0x1
} MAX77960_B2SOVRC_DTC_t;

typedef enum {
	MAX77960_TO_TIME_30SEC		= 0x00,
	MAX77960_TO_TIME_10MIN		= 0x01,
	MAX77960_TO_TIME_20MIN		= 0x02,
	MAX77960_TO_TIME_30MIN		= 0x03,
	MAX77960_TO_TIME_40MIN		= 0x04,
	MAX77960_TO_TIME_50MIN		= 0x05,
	MAX77960_TO_TIME_60MIN		= 0x06,
	MAX77960_TO_TIME_70MIN		= 0x07
} MAX77960_TO_TIME_t;

typedef enum {
	MAX77960_TO_ITH_100MA		= 0x00,
	MAX77960_TO_ITH_200MA,
	MAX77960_TO_ITH_300MA,
	MAX77960_TO_ITH_400MA,
	MAX77960_TO_ITH_500MA,
	MAX77960_TO_ITH_600MA,
    MAX77960_TO_ITH_600MA_1,    //same as 600MA
    MAX77960_TO_ITH_600MA_2,    //same as 600MA
} MAX77960_TO_ITH_t;

/*
 * Charger configuration 4
 */

typedef enum {
	MAX77960_CHG_CV_PRM_2S_8_00V        = 0x00,
    MAX77960_CHG_CV_PRM_2S_8_02V        = 0x01,
    MAX77960_CHG_CV_PRM_2S_8_04V        = 0x02,
    MAX77960_CHG_CV_PRM_2S_8_06V        = 0x03,
    MAX77960_CHG_CV_PRM_2S_8_08V        = 0x04,
    MAX77960_CHG_CV_PRM_2S_8_10V        = 0x05,
    MAX77960_CHG_CV_PRM_2S_8_12V        = 0x06,
    MAX77960_CHG_CV_PRM_2S_8_14V        = 0x07,
    MAX77960_CHG_CV_PRM_2S_8_16V        = 0x08,
    MAX77960_CHG_CV_PRM_2S_8_18V        = 0x09,
    MAX77960_CHG_CV_PRM_2S_8_20V        = 0x0A,
    MAX77960_CHG_CV_PRM_2S_8_22V        = 0x0B,
    MAX77960_CHG_CV_PRM_2S_8_24V        = 0x0C,
    MAX77960_CHG_CV_PRM_2S_8_26V        = 0x0D,
    MAX77960_CHG_CV_PRM_2S_8_28V        = 0x0E,
    MAX77960_CHG_CV_PRM_2S_8_30V        = 0x0F,
    MAX77960_CHG_CV_PRM_2S_8_32V        = 0x10,
    MAX77960_CHG_CV_PRM_2S_8_34V        = 0x11,
    MAX77960_CHG_CV_PRM_2S_8_36V        = 0x12,
    MAX77960_CHG_CV_PRM_2S_8_38V        = 0x13,
    MAX77960_CHG_CV_PRM_2S_8_40V        = 0x14,
    MAX77960_CHG_CV_PRM_2S_8_42V        = 0x15,
    MAX77960_CHG_CV_PRM_2S_8_44V        = 0x16,
    MAX77960_CHG_CV_PRM_2S_8_46V        = 0x17,
    MAX77960_CHG_CV_PRM_2S_8_48V        = 0x18,
    MAX77960_CHG_CV_PRM_2S_8_50V        = 0x19,
    MAX77960_CHG_CV_PRM_2S_8_52V        = 0x1A,
    MAX77960_CHG_CV_PRM_2S_8_54V        = 0x1B,
    MAX77960_CHG_CV_PRM_2S_8_56V        = 0x1C,
    MAX77960_CHG_CV_PRM_2S_8_58V        = 0x1D,
    MAX77960_CHG_CV_PRM_2S_8_60V        = 0x1E,
    MAX77960_CHG_CV_PRM_2S_8_62V        = 0x1F,
    MAX77960_CHG_CV_PRM_2S_8_64V        = 0x20,
    MAX77960_CHG_CV_PRM_2S_8_66V        = 0x21,
    MAX77960_CHG_CV_PRM_2S_8_68V        = 0x22,
    MAX77960_CHG_CV_PRM_2S_8_70V        = 0x23,
    MAX77960_CHG_CV_PRM_2S_8_72V        = 0x24,
    MAX77960_CHG_CV_PRM_2S_8_74V        = 0x25,
    MAX77960_CHG_CV_PRM_2S_8_76V        = 0x26,
    MAX77960_CHG_CV_PRM_2S_8_78V        = 0x27,
    MAX77960_CHG_CV_PRM_2S_8_80V        = 0x28,
    MAX77960_CHG_CV_PRM_2S_8_82V        = 0x29,
    MAX77960_CHG_CV_PRM_2S_8_84V        = 0x2A,
    MAX77960_CHG_CV_PRM_2S_8_86V        = 0x2B,
    MAX77960_CHG_CV_PRM_2S_8_88V        = 0x2C,
    MAX77960_CHG_CV_PRM_2S_8_90V        = 0x2D,
    MAX77960_CHG_CV_PRM_2S_8_92V        = 0x2E,
    MAX77960_CHG_CV_PRM_2S_8_94V        = 0x2F,
    MAX77960_CHG_CV_PRM_2S_8_96V        = 0x30,
    MAX77960_CHG_CV_PRM_2S_8_98V        = 0x31,
    MAX77960_CHG_CV_PRM_2S_9_00V        = 0x32,
    MAX77960_CHG_CV_PRM_2S_9_02V        = 0x33,
    MAX77960_CHG_CV_PRM_2S_9_04V        = 0x34,
    MAX77960_CHG_CV_PRM_2S_9_06V        = 0x35,
    MAX77960_CHG_CV_PRM_2S_9_08V        = 0x36,
    MAX77960_CHG_CV_PRM_2S_9_10V        = 0x37,
    MAX77960_CHG_CV_PRM_2S_9_12V        = 0x38,
    MAX77960_CHG_CV_PRM_2S_9_14V        = 0x39,
    MAX77960_CHG_CV_PRM_2S_9_16V        = 0x3A,
    MAX77960_CHG_CV_PRM_2S_9_18V        = 0x3B,
    MAX77960_CHG_CV_PRM_2S_9_20V        = 0x3C,
    MAX77960_CHG_CV_PRM_2S_9_22V        = 0x3D,
    MAX77960_CHG_CV_PRM_2S_9_24V        = 0x3E,
    MAX77960_CHG_CV_PRM_2S_9_26V        = 0x3F,
} MAX77960_CHG_CV_PRM_2S_t;

typedef enum {
	MAX77960_CHG_CV_PRM_3S_12_00V       = 0x00,
    MAX77960_CHG_CV_PRM_3S_12_03V       = 0x01,
    MAX77960_CHG_CV_PRM_3S_12_06V       = 0x02,
    MAX77960_CHG_CV_PRM_3S_12_09V       = 0x03,
    MAX77960_CHG_CV_PRM_3S_12_12V       = 0x04,
    MAX77960_CHG_CV_PRM_3S_12_15V       = 0x05,
    MAX77960_CHG_CV_PRM_3S_12_18V       = 0x06,
    MAX77960_CHG_CV_PRM_3S_12_21V       = 0x07,
    MAX77960_CHG_CV_PRM_3S_12_24V       = 0x08,
    MAX77960_CHG_CV_PRM_3S_12_27V       = 0x09,
    MAX77960_CHG_CV_PRM_3S_12_30V       = 0x0A,
    MAX77960_CHG_CV_PRM_3S_12_33V       = 0x0B,
    MAX77960_CHG_CV_PRM_3S_12_36V       = 0x0C,
    MAX77960_CHG_CV_PRM_3S_12_39V       = 0x0D,
    MAX77960_CHG_CV_PRM_3S_12_42V       = 0x0E,
    MAX77960_CHG_CV_PRM_3S_12_45V       = 0x0F,
    MAX77960_CHG_CV_PRM_3S_12_48V       = 0x10,
    MAX77960_CHG_CV_PRM_3S_12_51V       = 0x11,
    MAX77960_CHG_CV_PRM_3S_12_54V       = 0x12,
    MAX77960_CHG_CV_PRM_3S_12_57V       = 0x13,
    MAX77960_CHG_CV_PRM_3S_12_60V       = 0x14,
    MAX77960_CHG_CV_PRM_3S_12_63V       = 0x15,
    MAX77960_CHG_CV_PRM_3S_12_66V       = 0x16,
    MAX77960_CHG_CV_PRM_3S_12_69V       = 0x17,
    MAX77960_CHG_CV_PRM_3S_12_72V       = 0x18,
    MAX77960_CHG_CV_PRM_3S_12_75V       = 0x19,
    MAX77960_CHG_CV_PRM_3S_12_78V       = 0x1A,
    MAX77960_CHG_CV_PRM_3S_12_81V       = 0x1B,
    MAX77960_CHG_CV_PRM_3S_12_84V       = 0x1C,
    MAX77960_CHG_CV_PRM_3S_12_87V       = 0x1D,
    MAX77960_CHG_CV_PRM_3S_12_90V       = 0x1E,
    MAX77960_CHG_CV_PRM_3S_12_93V       = 0x1F,
    MAX77960_CHG_CV_PRM_3S_12_96V       = 0x20,
    MAX77960_CHG_CV_PRM_3S_12_99V       = 0x21,
    MAX77960_CHG_CV_PRM_3S_13_02V       = 0x22,
    MAX77960_CHG_CV_PRM_3S_13_05V       = 0x23
} MAX77960_CHG_CV_PRM_3S_t;

/*
 * Charger configuration 5
 */

typedef enum {
	MAX77960_ITRICKLE_100MA         = 0x00,
    MAX77960_ITRICKLE_200MA         = 0x01,
    MAX77960_ITRICKLE_300MA         = 0x02,
    MAX77960_ITRICKLE_400MA         = 0x03,
} MAX77960_ITRICKLE_t;

typedef enum {
	MAX77960_B2SOVRC_DISABLED       = 0x00,
    MAX77960_B2SOVRC_3_0A           = 0x01,
    MAX77960_B2SOVRC_3_5A           = 0x02,
    MAX77960_B2SOVRC_4_0A           = 0x03,
    MAX77960_B2SOVRC_4_5A           = 0x04,
    MAX77960_B2SOVRC_5_0A           = 0x05,
    MAX77960_B2SOVRC_5_5A           = 0x06,
    MAX77960_B2SOVRC_6_0A           = 0x07,
    MAX77960_B2SOVRC_6_5A           = 0x08,
    MAX77960_B2SOVRC_7_0A           = 0x09,
    MAX77960_B2SOVRC_7_5A           = 0x0A,
    MAX77960_B2SOVRC_8_0A           = 0x0B,
    MAX77960_B2SOVRC_8_5A           = 0x0C,
    MAX77960_B2SOVRC_9_0A           = 0x0D,
    MAX77960_B2SOVRC_9_5A           = 0x0E,
    MAX77960_B2SOVRC_10_0A          = 0x0F
} MAX77960_B2SOVRC_t;

/*
 * Charger configuration 6
 */

typedef enum {
	MAX77960_CHGPROT_LOCKED_0       = 0x00,
    MAX77960_CHGPROT_LOCKED_1       = 0x01,
    MAX77960_CHGPROT_LOCKED_2       = 0x02,
    MAX77960_CHGPROT_UNLOCKED       = 0x03
} MAX77960_CHGPROT_t;

typedef enum {
	MAX77960_WDTCLR_NO_CLEAR_0      = 0x00,
    MAX77960_WDTCLR_CLEAR           = 0x01,
    MAX77960_WDTCLR_NO_CLEAR_1      = 0x02,
    MAX77960_WDTCLR_NO_CLEAR_2      = 0x03
} MAX77960_WDTCLR_t;

/*
 * Charger configuration 7
 */

typedef enum {
	MAX77960_REGTEMP_85_C       = 0x0,
    MAX77960_REGTEMP_90_C       = 0x1,
    MAX77960_REGTEMP_95_C       = 0x2,
    MAX77960_REGTEMP_100_C      = 0x3,
    MAX77960_REGTEMP_105_C      = 0x4,
    MAX77960_REGTEMP_110_C      = 0x5,
    MAX77960_REGTEMP_115_C      = 0x6,
    MAX77960_REGTEMP_120_C      = 0x7,
    MAX77960_REGTEMP_125_C      = 0x8,
    MAX77960_REGTEMP_130_C      = 0x9
} MAX77960_REGTEMP_t;

/*
 * Charger configuration 8
 */

typedef enum {
	MAX77960_CHGIN_ILIM_100MA_0     = 0x00,
    MAX77960_CHGIN_ILIM_100MA_1     = 0x01,   //same as 0x00
    MAX77960_CHGIN_ILIM_100MA_2     = 0x02,   //same as 0x00
    MAX77960_CHGIN_ILIM_100MA_3     = 0x03,   //same as 0x00
    MAX77960_CHGIN_ILIM_150MA       = 0x04,
    MAX77960_CHGIN_ILIM_200MA       = 0x05,
    MAX77960_CHGIN_ILIM_250MA       = 0x06,
    MAX77960_CHGIN_ILIM_300MA       = 0x07,
    MAX77960_CHGIN_ILIM_350MA       = 0x08,
    MAX77960_CHGIN_ILIM_400MA       = 0x09,
    MAX77960_CHGIN_ILIM_450MA       = 0x0A,
    MAX77960_CHGIN_ILIM_500MA       = 0x0B,
    MAX77960_CHGIN_ILIM_550MA       = 0x0C,
    MAX77960_CHGIN_ILIM_600MA       = 0x0D,
    MAX77960_CHGIN_ILIM_650MA       = 0x0E,
    MAX77960_CHGIN_ILIM_700MA       = 0x0F,
    MAX77960_CHGIN_ILIM_750MA       = 0x10,
    MAX77960_CHGIN_ILIM_800MA       = 0x11,
    MAX77960_CHGIN_ILIM_850MA       = 0x12,
    MAX77960_CHGIN_ILIM_900MA       = 0x13,
    MAX77960_CHGIN_ILIM_950MA       = 0x14,
    MAX77960_CHGIN_ILIM_1000MA      = 0x15,
    MAX77960_CHGIN_ILIM_1050MA      = 0x16,
    MAX77960_CHGIN_ILIM_1100MA      = 0x17,
    MAX77960_CHGIN_ILIM_1150MA      = 0x18,
    MAX77960_CHGIN_ILIM_1200MA      = 0x19,
    MAX77960_CHGIN_ILIM_1250MA      = 0x1A,
    MAX77960_CHGIN_ILIM_1300MA      = 0x1B,
    MAX77960_CHGIN_ILIM_1350MA      = 0x1C,
    MAX77960_CHGIN_ILIM_1400MA      = 0x1D,
    MAX77960_CHGIN_ILIM_1450MA      = 0x1E,
    MAX77960_CHGIN_ILIM_1500MA      = 0x1F,
    MAX77960_CHGIN_ILIM_1550MA      = 0x20,
    MAX77960_CHGIN_ILIM_1600MA      = 0x21,
    MAX77960_CHGIN_ILIM_1650MA      = 0x22,
    MAX77960_CHGIN_ILIM_1700MA      = 0x23,
    MAX77960_CHGIN_ILIM_1750MA      = 0x24,
    MAX77960_CHGIN_ILIM_1800MA      = 0x25,
    MAX77960_CHGIN_ILIM_1850MA      = 0x26,
    MAX77960_CHGIN_ILIM_1900MA      = 0x27,
    MAX77960_CHGIN_ILIM_1950MA      = 0x28,
    MAX77960_CHGIN_ILIM_2000MA      = 0x29,
    MAX77960_CHGIN_ILIM_2050MA      = 0x2A,
    MAX77960_CHGIN_ILIM_2100MA      = 0x2B,
    MAX77960_CHGIN_ILIM_2150MA      = 0x2C,
    MAX77960_CHGIN_ILIM_2200MA      = 0x2D,
    MAX77960_CHGIN_ILIM_2250MA      = 0x2E,
    MAX77960_CHGIN_ILIM_2300MA      = 0x2F,
    MAX77960_CHGIN_ILIM_2350MA      = 0x30,
    MAX77960_CHGIN_ILIM_2400MA      = 0x31,
    MAX77960_CHGIN_ILIM_2450MA      = 0x32,
    MAX77960_CHGIN_ILIM_2500MA      = 0x33,
    MAX77960_CHGIN_ILIM_2550MA      = 0x34,
    MAX77960_CHGIN_ILIM_2600MA      = 0x35,
    MAX77960_CHGIN_ILIM_2650MA      = 0x36,
    MAX77960_CHGIN_ILIM_2700MA      = 0x37,
    MAX77960_CHGIN_ILIM_2750MA      = 0x38,
    MAX77960_CHGIN_ILIM_2800MA      = 0x39,
    MAX77960_CHGIN_ILIM_2850MA      = 0x3A,
    MAX77960_CHGIN_ILIM_2900MA      = 0x3B,
    MAX77960_CHGIN_ILIM_2950MA      = 0x3C,
    MAX77960_CHGIN_ILIM_3000MA      = 0x3D,
    MAX77960_CHGIN_ILIM_3050MA      = 0x3E,
    MAX77960_CHGIN_ILIM_3100MA      = 0x3F,
    MAX77960_CHGIN_ILIM_3150MA      = 0x40,
    MAX77960_CHGIN_ILIM_3200MA      = 0x41,
    MAX77960_CHGIN_ILIM_3250MA      = 0x42,
    MAX77960_CHGIN_ILIM_3300MA      = 0x43,
    MAX77960_CHGIN_ILIM_3350MA      = 0x44,
    MAX77960_CHGIN_ILIM_3400MA      = 0x45,
    MAX77960_CHGIN_ILIM_3450MA      = 0x46,
    MAX77960_CHGIN_ILIM_3500MA      = 0x47,
    MAX77960_CHGIN_ILIM_3550MA      = 0x48,
    MAX77960_CHGIN_ILIM_3600MA      = 0x49,
    MAX77960_CHGIN_ILIM_3650MA      = 0x4A,
    MAX77960_CHGIN_ILIM_3700MA      = 0x4B,
    MAX77960_CHGIN_ILIM_3750MA      = 0x4C,
    MAX77960_CHGIN_ILIM_3800MA      = 0x4D,
    MAX77960_CHGIN_ILIM_3850MA      = 0x4E,
    MAX77960_CHGIN_ILIM_3900MA      = 0x4F,
    MAX77960_CHGIN_ILIM_3950MA      = 0x50,
    MAX77960_CHGIN_ILIM_4000MA      = 0x51,
    MAX77960_CHGIN_ILIM_4050MA      = 0x52,
    MAX77960_CHGIN_ILIM_4100MA      = 0x53,
    MAX77960_CHGIN_ILIM_4150MA      = 0x54,
    MAX77960_CHGIN_ILIM_4200MA      = 0x55,
    MAX77960_CHGIN_ILIM_4250MA      = 0x56,
    MAX77960_CHGIN_ILIM_4300MA      = 0x57,
    MAX77960_CHGIN_ILIM_4350MA      = 0x58,
    MAX77960_CHGIN_ILIM_4400MA      = 0x59,
    MAX77960_CHGIN_ILIM_4450MA      = 0x5A,
    MAX77960_CHGIN_ILIM_4500MA      = 0x5B,
    MAX77960_CHGIN_ILIM_4550MA      = 0x5C,
    MAX77960_CHGIN_ILIM_4600MA      = 0x5D,
    MAX77960_CHGIN_ILIM_4650MA      = 0x5E,
    MAX77960_CHGIN_ILIM_4700MA      = 0x5F,
    MAX77960_CHGIN_ILIM_4750MA      = 0x60,
    MAX77960_CHGIN_ILIM_4800MA      = 0x61,
    MAX77960_CHGIN_ILIM_4850MA      = 0x62,
    MAX77960_CHGIN_ILIM_4900MA      = 0x63,
    MAX77960_CHGIN_ILIM_4950MA      = 0x64,
    MAX77960_CHGIN_ILIM_5000MA      = 0x65,
    MAX77960_CHGIN_ILIM_5050MA      = 0x66,
    MAX77960_CHGIN_ILIM_5100MA      = 0x67,
    MAX77960_CHGIN_ILIM_5150MA      = 0x68,
    MAX77960_CHGIN_ILIM_5200MA      = 0x69,
    MAX77960_CHGIN_ILIM_5250MA      = 0x6A,
    MAX77960_CHGIN_ILIM_5300MA      = 0x6B,
    MAX77960_CHGIN_ILIM_5350MA      = 0x6C,
    MAX77960_CHGIN_ILIM_5400MA      = 0x6D,
    MAX77960_CHGIN_ILIM_5450MA      = 0x6E,
    MAX77960_CHGIN_ILIM_5500MA      = 0x6F,
    MAX77960_CHGIN_ILIM_5550MA      = 0x70,
    MAX77960_CHGIN_ILIM_5600MA      = 0x71,
    MAX77960_CHGIN_ILIM_5650MA      = 0x72,
    MAX77960_CHGIN_ILIM_5700MA      = 0x73,
    MAX77960_CHGIN_ILIM_5750MA      = 0x74,
    MAX77960_CHGIN_ILIM_5800MA      = 0x75,
    MAX77960_CHGIN_ILIM_5850MA      = 0x76,
    MAX77960_CHGIN_ILIM_5900MA      = 0x77,
    MAX77960_CHGIN_ILIM_5950MA      = 0x78,
    MAX77960_CHGIN_ILIM_6000MA      = 0x79,
    MAX77960_CHGIN_ILIM_6050MA      = 0x7A,
    MAX77960_CHGIN_ILIM_6100MA      = 0x7B,
    MAX77960_CHGIN_ILIM_6150MA      = 0x7C,
    MAX77960_CHGIN_ILIM_6200MA      = 0x7D,
    MAX77960_CHGIN_ILIM_6250MA      = 0x7E,
    MAX77960_CHGIN_ILIM_6300MA      = 0x7F,
} MAX77960_CHGIN_ILIM_t;

/*
 * Charger configuration 8
 */

typedef enum {
	MAX77960_INLIM_CLK_8_US         = 0x0,
    MAX77960_INLIM_CLK_256_US       = 0x1,
    MAX77960_INLIM_CLK_1024_US      = 0x2,
    MAX77960_INLIM_CLK_4096_US      = 0x3
} MAX77960_INLIM_CLK_t;

typedef enum {
	MAX77960_OTG_ILIM_500MA         = 0x00,
	MAX77960_OTG_ILIM_900MA         = 0x01,
    MAX77960_OTG_ILIM_1200MA        = 0x02,
    MAX77960_OTG_ILIM_1500MA        = 0x03,
    MAX77960_OTG_ILIM_2000MA        = 0x04,
    MAX77960_OTG_ILIM_2250MA        = 0x05,
    MAX77960_OTG_ILIM_2500MA        = 0x06,
    MAX77960_OTG_ILIM_3000MA        = 0x07
} MAX77960_OTG_ILIM_t;

typedef enum {
	MAX77960_MINVSYS_2S_5535MV         = 0x00,
	MAX77960_MINVSYS_2S_5740MV         = 0x01,
    MAX77960_MINVSYS_2S_5945MV         = 0x02,
    MAX77960_MINVSYS_2S_6150MV         = 0x03,
    MAX77960_MINVSYS_2S_6335MV         = 0x04,
    MAX77960_MINVSYS_2S_6560MV         = 0x05,
    MAX77960_MINVSYS_2S_6765MV         = 0x06,
    MAX77960_MINVSYS_2S_6970MV         = 0x07,
} MAX77960_MINVSYS_2S_t;

typedef enum {
	MAX77960_MINVSYS_3S_8303MV         = 0x00,
    MAX77960_MINVSYS_3S_8610MV         = 0x01,
    MAX77960_MINVSYS_3S_8918MV         = 0x02,
    MAX77960_MINVSYS_3S_9225MV         = 0x03,
    MAX77960_MINVSYS_3S_9533MV         = 0x04,
    MAX77960_MINVSYS_3S_9840MV         = 0x05,
    MAX77960_MINVSYS_3S_10148MV        = 0x06,
    MAX77960_MINVSYS_3S_10455MV        = 0x07
} MAX77960_MINVSYS_3S_t;

/*
 * Charger configuration 8
 */

typedef enum {
	MAX77960_VCHGIN_REG_4025MV         = 0x00,
    MAX77960_VCHGIN_REG_4200MV         = 0x01,
    MAX77960_VCHGIN_REG_4375MV         = 0x02,
    MAX77960_VCHGIN_REG_4550MV         = 0x03,
    MAX77960_VCHGIN_REG_4725MV         = 0x04,
    MAX77960_VCHGIN_REG_4900MV         = 0x05,
    MAX77960_VCHGIN_REG_5425MV         = 0x06,
    MAX77960_VCHGIN_REG_5950MV         = 0x07,
    MAX77960_VCHGIN_REG_6475MV         = 0x08,
    MAX77960_VCHGIN_REG_7000MV         = 0x09,
    MAX77960_VCHGIN_REG_7525MV         = 0x0A,
    MAX77960_VCHGIN_REG_8050MV         = 0x0B,
    MAX77960_VCHGIN_REG_8575MV         = 0x0C,
    MAX77960_VCHGIN_REG_9100MV         = 0x0D,
    MAX77960_VCHGIN_REG_9625MV         = 0x0E,
    MAX77960_VCHGIN_REG_10150MV        = 0x0F,
    MAX77960_VCHGIN_REG_10675MV        = 0x10,
    MAX77960_VCHGIN_REG_10950MV        = 0x11,
    MAX77960_VCHGIN_REG_11550MV        = 0x12,
    MAX77960_VCHGIN_REG_12150MV        = 0x13,
    MAX77960_VCHGIN_REG_12750MV        = 0x14,
    MAX77960_VCHGIN_REG_13350MV        = 0x15,
    MAX77960_VCHGIN_REG_13950MV        = 0x16,
    MAX77960_VCHGIN_REG_14550MV        = 0x17,
    MAX77960_VCHGIN_REG_15150MV        = 0x18,
    MAX77960_VCHGIN_REG_15750MV        = 0x19,
    MAX77960_VCHGIN_REG_16350MV        = 0x1A,
    MAX77960_VCHGIN_REG_16950MV        = 0x1B,
    MAX77960_VCHGIN_REG_17550MV        = 0x1C,
    MAX77960_VCHGIN_REG_18150MV        = 0x1D,
    MAX77960_VCHGIN_REG_18750MV        = 0x1E,
    MAX77960_VCHGIN_REG_19050MV        = 0x1F
} MAX77960_VCHGIN_REG_t;

// ----------------------------------------------------------------

/**
 * MAX77960 Fault type enum
 */
typedef enum{
    THM_SHUTDOWN        = 0,
    TIMER_FAULT         = 1,
    WDT_FAULT           = 2,
    BATT_OVERCURRENT    = 3,
    VSYS_FAULT          = 4,    
    OTG_FAULT           = 5     //OTG_PLIM_OK 
} MAX77960_FAULT_TYPE_t;

/**
 * MAX77960 Charge config structure
 */
typedef struct{
    MAX77960_CHGIN_ILIM_t INLIM;
    MAX77960_INLIM_CLK_t INLIM_CLK_;    //input current soft start period between increments of 25mA in uS
	MAX77960_OTG_ILIM_t OTG_ILIM_;      //OTG current limit
    MAX77960_CHG_CHGCC_t ISET;
    MAX77960_TO_ITH_t ITO;
	MAX77960_CHG_CV_PRM_2S_t VSET_2S;   //VSET for 2S mode
    MAX77960_CHG_CV_PRM_3S_t VSET_3S;   //VSET for 3S mode
    MAX77960_ITRICKLE_t I_TRICKLE;
    MAX77960_TO_TIME_t  TO_timer;
    MAX77960_FCHGTIME_t FC_timer;
    MAX77960_CHG_RSTRT_t CHG_RST;       // restart charging threshold below VSET
    bool JEITA_enabled;                 //JEITA function enabled = 1, disabled = 0
} MAX77960_CHG_CONFIG_t;

/**
 * MAX77960 General config structure
 */
typedef struct{
    MAX77960_B2SOVRC_t B2SOVRC_;        //battery to sys overcurrent threshold
    MAX77960_MINVSYS_2S_t MINVSYS_2S;   //minimum system voltage threshold for 2S mode
    MAX77960_MINVSYS_3S_t MINVSYS_3S;   //minimum system voltage threshold for 3S mode
    MAX77960_REGTEMP_t REGTEMP_;        //junction temperature regulation threshold
    MAX77960_VCHGIN_REG_t VCHGIN_REG_;  //Charger Input Voltage Setting, ignored if AICL enabled
} MAX77960_CONFIG_t;

/**
 * MAX77960 Details structure
 */
typedef struct{
    MAX77960_CHGIN_DTLS_t CHGIN_det;
    MAX77960_OTG_DTLS_t OTG_det;
    MAX77960_BAT_DTLS_t BAT_det;
    bool QB_enabled;
    MAX77960_CHG_DTLS_t CHG_det;
    MAX77960_THM_DTLS_t THM_det;
    bool CHG_enabled;
    MAX77960_CHG_MODE_t mode;           //Smart Power Selector Configurator Mode
} MAX77960_DETAILS_t;

/**
 * MAX77960 Details Strings Structure
 */
typedef struct{
    const char *CHGIN_det_s;
    const char *OTG_det_s;
    const char *BAT_det_s;
    const char *QB_enabled_s;
    const char *CHG_det_s;
    const char *THM_det_s;
    const char *CHG_enabled_s;
    const char *mode_s;
} MAX77960_DETAILS_STRINGS_t;

/**
 * MAX77960 Stucture
 */
typedef struct {

    uint8_t i2c_port;

    bool _is_init;
    bool _is_configured;
    bool _is_charge_configured;
    
    //status and info
    MAX77960_FSW_DTLS_t FSW;            //Switching frequency of the chip
    MAX77960_NUM_CELL_DTLS_t NUM_CELL;  //max number of cells supported by the chip: 0=2S, 1=3S
    //TODO: implements flags
    bool is_CHGIN_valid;                //charger input voltage
    bool is_CHGIN_INLIM_reached;        //charger input voltage reached
    bool is_charging;
    bool is_fault;                      //is_fault=1 if fault
    bool DISQBAT_enabled;               //Qbat disabled = 0, enabled = 1
    
    MAX77960_CHGPROT_t chgprot_state;
    MAX77960_DETAILS_t details;
    MAX77960_FAULT_TYPE_t fault_type;   //fault type

} ESP32_MAX77960;


void MAX_init(ESP32_MAX77960 * MAX, uint8_t i2c_port);

void MAX_set_chg_conf(ESP32_MAX77960* MAX, MAX77960_CHG_CONFIG_t chg_conf);
void MAX_set_conf(ESP32_MAX77960* MAX, MAX77960_CONFIG_t conf);
void MAX_set_mode(ESP32_MAX77960* MAX, MAX77960_CHG_MODE_t mode);
void MAX_set_chg_i2c_mode(ESP32_MAX77960 * MAX);
MAX77960_DETAILS_STRINGS_t MAX_get_details(ESP32_MAX77960* MAX);

bool MAX_charger_protection_unlock(ESP32_MAX77960* MAX);
bool MAX_charger_protection_lock(ESP32_MAX77960* MAX);

/*
The chip has different levels of reset as defined below:
• Type S: Registers are reset each time when: SYS < VDD (1.8V)
• Type O: Registers are reset each time when: SYS < VDD or SYS < SYS UVLO or SYS > SYS OVLO or die temp >
TSHDN or software reset (SW_RST)

Type S registers: all of TOP section, and all of the charger interrupt/details except CHG_INT_MASK are type S.

Type O registers: CHG_INT_MASK and all charger configuration registers are type O.
*/
void MAX_software_reset(ESP32_MAX77960* MAX);

//TODO: Interrupt functions, remember to implement the return of the fault type


#endif
