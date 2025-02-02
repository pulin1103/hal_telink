/********************************************************************************************************
 * @file    pm.h
 *
 * @brief   This is the header file for B95
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#include "reg_include/register.h"
#include "compiler.h"
#include "gpio.h"
#include "clock.h"

/********************************************************************************************************
 *											internal
 *******************************************************************************************************/

/********************************************************************************************************
 * 				This is currently included in the H file for compatibility with other SDKs.
 *******************************************************************************************************/

//When the watchdog comes back, the Eagle chip does not clear 0x7f[0]. To avoid this problem, this macro definition is added.
#ifndef WDT_REBOOT_RESET_ANA7F_WORK_AROUND
#define WDT_REBOOT_RESET_ANA7F_WORK_AROUND	1
#endif

/********************************************************************************************************
 *											external
 *******************************************************************************************************/


/**
 * @brief these analog register can store data in deep sleep mode or deep sleep with SRAM retention mode.
 * 	      Reset these analog registers by watchdog, software reboot (sys_reboot()), RESET Pin, power cycle, 32k watchdog, vbus detect.
 */
#define PM_ANA_REG_WD_CLR_BUF0			0x35 // initial value 0xff. [Bit0] is already occupied. The customer cannot change!
#define PM_ANA_REG_WD_CLR_BUF1			0x36 // initial value 0x00.
#define PM_ANA_REG_WD_CLR_BUF2			0x37 // initial value 0x00
#define PM_ANA_REG_WD_CLR_BUF3			0x38 // initial value 0x00
#define PM_ANA_REG_WD_CLR_BUF4			0x39 // initial value 0x00

/**
 * @brief analog register below can store information when MCU in deep sleep mode or deep sleep with SRAM retention mode.
 * 	      Reset these analog registers by power cycle, 32k watchdog, RESET Pin,vbus detect.
 */
/**
 * Customers cannot use [bit0],[bit1],[bit2],[bit7] of analog register 0x3a because driver and chip functions are occupied, details are as follows:
 * [Bit0]: If this bit is 1, it means that reboot has occurred.
 * [Bit1]: If this bit is 1, it means that the software calls the function sys_reboot() when the crystal oscillator does not start up normally.
 * [Bit2]: If this bit is 1, it means that the pm_sleep_wakeup function failed to clear the pm wake flag bit when using the deep wake source, and the software called sys_reboot().
 * [Bit7]: The security boot function of the bootrom is used.
 */
#define PM_ANA_REG_POWER_ON_CLR_BUF0	0x3a // initial value 0x00
#define PM_ANA_REG_POWER_ON_CLR_BUF1	0x3b // initial value 0x00
#define PM_ANA_REG_POWER_ON_CLR_BUF2	0x3c // initial value 0xff

/**
 * @brief	gpio wakeup level definition
 */
typedef enum{
	WAKEUP_LEVEL_LOW		= 0,
	WAKEUP_LEVEL_HIGH		= 1,
}pm_gpio_wakeup_level_e;

/**
 * @brief	wakeup tick type definition
 */
typedef enum {
	 PM_TICK_STIMER			= 0,	// 24M
	 PM_TICK_32K			= 1,
}pm_wakeup_tick_type_e;

/**
 * @brief	suspend power whether to power down definition
 */
typedef enum {
	 PM_POWER_BASEBAND  = BIT(0),	//whether to power on the BASEBAND before suspend.
	 PM_POWER_USB  		= BIT(1),	//whether to power on the USB before suspend.
	 PM_POWER_AUDIO 	= BIT(2),	//weather to power on the AUDIO before suspend.
}pm_suspend_power_cfg_e;

/**
 * @brief	active mode VDDO3 output trim definition
 * @note	The voltage values of the following gears are all theoretical values, and there may be deviations between the actual and theoretical values.
 */
typedef enum {
	PM_VDDO3_VOLTAGE_1V4_2V9	= 0x00, /**<LDO output 1.4V in 1.8V mode & 2.9V in 3.3V mode */
	PM_VDDO3_VOLTAGE_1V5_3V0	= 0x01, /**<LDO output 1.5V in 1.8V mode & 3.0V in 3.3V mode */
	PM_VDDO3_VOLTAGE_1V6_3V1	= 0x02, /**<LDO output 1.6V in 1.8V mode & 3.1V in 3.3V mode */
	PM_VDDO3_VOLTAGE_1V7_3V2	= 0x03, /**<LDO output 1.7V in 1.8V mode & 3.2V in 3.3V mode */
	PM_VDDO3_VOLTAGE_1V8_3V3	= 0x04, /**<LDO output 1.8V in 1.8V mode & 3.3V in 3.3V mode */
	PM_VDDO3_VOLTAGE_1V9_3V4	= 0x05, /**<LDO output 1.9V in 1.8V mode & 3.4V in 3.3V mode */
	PM_VDDO3_VOLTAGE_2V0_3V5	= 0x06, /**<LDO output 2.0V in 1.8V mode & 3.5V in 3.3V mode */
	PM_VDDO3_VOLTAGE_2V1_3V6	= 0x07, /**<LDO output 2.1V in 1.8V mode & 3.6V in 3.3V mode */
}pm_vddo3_voltage_e;

/**
 * @brief	sleep mode.
 */
typedef enum {
	//available mode for customer
	SUSPEND_MODE						= 0x00,
	DEEPSLEEP_MODE						= 0xf0,	//when use deep mode pad wakeup(low or high level), if the high(low) level always in the pad,
												//system will not enter sleep and go to below of pm API, will reboot by core_6f = 0x20.
												//deep retention also had this issue, but not to reboot.
	DEEPSLEEP_MODE_RET_SRAM_LOW32K  	= 0x01, //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW64K  	= 0x03, //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW128K  	= 0x07, //for boot from sram
	DEEPSLEEP_MODE_RET_SRAM_LOW256K  	= 0x0f, //for boot from sram
	//not available mode
	DEEPSLEEP_RETENTION_FLAG			= 0x0F,
}pm_sleep_mode_e;

/**
 * @brief	available wake-up source for customer
 */
typedef enum {
	 PM_WAKEUP_PAD   		= BIT(0),
	 PM_WAKEUP_CORE  		= BIT(1),
	 PM_WAKEUP_TIMER 		= BIT(2),
	 PM_WAKEUP_COMPARATOR 	= BIT(3),	/**<
											There are two things to note when using LPC wake up:
											1.After the LPC is configured, you need to wait 100 microseconds before go to sleep because the LPC need 1-2 32k tick to calculate the result.
											  If you enter the sleep function at this time, you may not be able to sleep normally because the data in the result register is abnormal.

											2.When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV, otherwise can not enter sleep normally, crash occurs.
										  */
//	 PM_WAKEUP_SHUTDOWN		= BIT(7),
}pm_sleep_wakeup_src_e;

/**
 * @brief	wakeup status
 */
typedef enum {
	WAKEUP_STATUS_PAD  				= BIT(0),
	WAKEUP_STATUS_CORE  			= BIT(1),
	WAKEUP_STATUS_TIMER 			= BIT(2),
	WAKEUP_STATUS_COMPARATOR    	= BIT(3),
	WAKEUP_STATUS_ALL  				= 0xff,

	STATUS_GPIO_ERR_NO_ENTER_PM		= BIT(8), /**<Bit8 is used to determine whether the wake source is normal.*/
	STATUS_CLEAR_FAIL	  			= BIT(29),
	STATUS_ENTER_SUSPEND  			= BIT(30),
}pm_wakeup_status_e;

/**
 * @brief   	zb voltage definition.
 */
typedef enum {
	ZB_0V8        =0x00,
	ZB_0V85       =0x20,
	ZB_0V9        =0x40,
	ZB_0V95       =0x60,
	ZB_1V         =0x80,
	ZB_1V05       =0xA0,
	ZB_1V1        =0xC0,
	ZB_1V15       =0xE0,
}pm_zb_voltage_e;

/**
 * @brief	mcu status
 */
typedef enum{
	MCU_STATUS_POWER_ON			= BIT(0), /**<	power on, vbus detect or reset pin */
	MCU_STATUS_REBOOT_BACK		= BIT(2), /**<	the reboot specific categories,see pm_reboot_event_e:
												1.If want to know which reboot it is, call the pm_get_mcu_reboot_status() interface to determine after calling sys_init().
												2.If determine whether is 32k watchdog/timer watchdog,can also use the interface wd_32k_get_status()/wd_get_status() to determine.
	 	 	 	 	 	 	 	 	 	 	 	*/
	MCU_STATUS_DEEPRET_BACK		= BIT(3),
	MCU_STATUS_DEEP_BACK		= BIT(4),
}pm_mcu_status;

/**
 * @brief  reboot status
 */
typedef enum{
	SW_SYSTEM_REBOOT			= BIT(0),/**< Clear the watchdog status flag in time, otherwise, the system reboot may be wrongly judged as the watchdog.*/
	HW_TIMER_WATCHDOG_REBOOT	= BIT(1),
	HW_32K_WATCHDOG_REBOOT		= BIT(2),/**< - When the 32k watchdog/timer watchdog status is set to 1, if it is not cleared:
	                                          - power cyele/vbus detect/reset pin come back, the status is lost;
                                              - but software reboot(sys_reboot())/deep/deepretation/32k watchdog come back,the status remains;
	                                          */
}pm_reboot_event_e;

/**
 * @brief digital module
 * 
 */
typedef enum{
	DIG_MODULE_ZB				= BIT(0),
	DIG_MODULE_USB				= BIT(1),
	DIG_MODULE_AUDIO			= BIT(2),
}pm_dig_module_e;

/**
 * @brief power sel
 * 
 */
typedef enum{
	PM_POWER_UP 		= 0,
	PM_POWER_DOWN	 	= 1,
}pm_power_sel_e;

/**
 * @brief dcdc trim flash out
 * 
 */
typedef enum{
	TRIM_1P8V_TO_1P64V = 0,
	TRIM_1P8V_TO_1P66V,
	TRIM_1P8V_TO_1P68V,
	TRIM_1P8V_TO_1P70V,
	TRIM_1P8V_TO_1P72V,
	TRIM_1P8V_TO_1P74V,
	TRIM_1P8V_TO_1P76V,
	TRIM_1P8V_TO_1P78V,
	TRIM_1P8V_TO_1P80V,
	TRIM_1P8V_TO_1P83V,
	TRIM_1P8V_TO_1P86V,
	TRIM_1P8V_TO_1P89V,
	TRIM_1P8V_TO_1P92V,
	TRIM_1P8V_TO_1P95V,
	TRIM_1P8V_TO_1P98V,
	TRIM_1P8V_TO_2P01V,
}pm_trim_1p8v_e;

/**
 * @brief dcdc trim soc out
 * 
 */
typedef enum{
	TRIM_0P94V_TO_0P820V = 0,
	TRIM_0P94V_TO_0P835V,
	TRIM_0P94V_TO_0P850V,
	TRIM_0P94V_TO_0P865V,
	TRIM_0P94V_TO_0P880V,
	TRIM_0P94V_TO_08950V,
	TRIM_0P94V_TO_0P910V,
	TRIM_0P94V_TO_0P925V,
	TRIM_0P94V_TO_0P940V,
	TRIM_0P94V_TO_0P963V,
	TRIM_0P94V_TO_0P986V,
	TRIM_0P94V_TO_1P009V,
	TRIM_0P94V_TO_1P032V,
	TRIM_0P94V_TO_1P055V,
	TRIM_0P94V_TO_1P078V,
	TRIM_0P94V_TO_1P101V,
}pm_trim_0p94v_e;

/**
 * @brief trim dig ldo
 * 
 */
typedef enum {
    DIG_LDO_TRIM_0P550V = 0,
    DIG_LDO_TRIM_0P575V,
    DIG_LDO_TRIM_0P600V,
    DIG_LDO_TRIM_0P625V,
    DIG_LDO_TRIM_0P650V,
    DIG_LDO_TRIM_0P675V,
    DIG_LDO_TRIM_0P700V,
    DIG_LDO_TRIM_0P725V,
    DIG_LDO_TRIM_0P750V,
    DIG_LDO_TRIM_0P775V,
    DIG_LDO_TRIM_0P800V,
    DIG_LDO_TRIM_0P825V,
    DIG_LDO_TRIM_0P850V,
    DIG_LDO_TRIM_0P875V,
    DIG_LDO_TRIM_0P900V,
    DIG_LDO_TRIM_0P925V,
} pm_dig_ldo_trim_e;

/**
 * @brief trim suspend ldo
 * 
 */
typedef enum {
	SPD_LDO_TRIM_0P55V = 0,
    SPD_LDO_TRIM_0P60V,
    SPD_LDO_TRIM_0P65V,
    SPD_LDO_TRIM_0P70V,
    SPD_LDO_TRIM_0P75V,
    SPD_LDO_TRIM_0P80V,
    SPD_LDO_TRIM_0P85V,
    SPD_LDO_TRIM_0P90V,
} pm_spd_ldo_trim_e;

/**
 * @brief trim dcore ldo
 * 
 */
typedef enum {
    DCORE_LDO_TRIM_0P550V = 0,
    DCORE_LDO_TRIM_0P575V,
    DCORE_LDO_TRIM_0P600V,
    DCORE_LDO_TRIM_0P625V,
    DCORE_LDO_TRIM_0P650V,
    DCORE_LDO_TRIM_0P675V,
    DCORE_LDO_TRIM_0P700V,
    DCORE_LDO_TRIM_0P725V,
    DCORE_LDO_TRIM_0P750V,
    DCORE_LDO_TRIM_0P775V,
    DCORE_LDO_TRIM_0P800V,
    DCORE_LDO_TRIM_0P825V,
    DCORE_LDO_TRIM_0P850V,
    DCORE_LDO_TRIM_0P875V,
    DCORE_LDO_TRIM_0P900V,
    DCORE_LDO_TRIM_0P925V,
} pm_dcore_ldo_trim_e;

/**
 * @brief trim sram ldo
 * 
 */
typedef enum {
    SRAM_LDO_TRIM_0P60V = 0,
    SRAM_LDO_TRIM_0P65V,
    SRAM_LDO_TRIM_0P70V,
    SRAM_LDO_TRIM_0P75V,
    SRAM_LDO_TRIM_0P80V,
    SRAM_LDO_TRIM_0P85V,
    SRAM_LDO_TRIM_0P90V,
    SRAM_LDO_TRIM_0P95V,
} pm_sram_ldo_trim_e;

/**
 * @brief power config
 * 
 */
typedef enum{
	CORE_0P9V_SRAM_0P9V_BB_0P9V = 0x00,	/**< dig ldo mode  1.05V-LDO/DCDC 0.9V_CORE 0.9V_SRAM 0.9V BB*/
	CORE_0P8V_SRAM_0P8V_BB_0P8V	= 0x01,	/**< dig ldo mode  0.94V-LDO/DCDC 0.8V_CORE 0.8V_SRAM 0.8V BB*/
	CORE_0P7V_SRAM_0P8V_BB_0P7V	= 0x02,	/**< multi ldo mode  0.94V-LDO/DCDC 0.7V_CORE 0.8V_SRAM 0.7V BB*/
}pm_power_cfg_e;

/**
 * @brief	early wakeup time
 */
typedef struct {
	unsigned short  suspend_early_wakeup_time_us;	/**< suspend_early_wakeup_time_us = deep_ret_r_delay_us + xtal_stable_time + early_time*/
	unsigned short  deep_ret_early_wakeup_time_us;  /**< deep_ret_early_wakeup_time_us = deep_ret_r_delay_us + early_time*/
	unsigned short  deep_early_wakeup_time_us;		/**< deep_early_wakeup_time_us = suspend_ret_r_delay_us*/
	unsigned short  sleep_min_time_us;				/**< sleep_min_time_us = suspend_early_wakeup_time_us + 200*/
}pm_early_wakeup_time_us_s;

extern volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;
/**
 * @brief	hardware delay time
 */
typedef struct {
	unsigned short  deep_r_delay_cycle ;			/**< hardware delay time ,deep_ret_r_delay_us = deep_r_delay_cycle * 1/16k */
	unsigned short  suspend_ret_r_delay_cycle ;		/**< hardware delay time ,suspend_ret_r_delay_us = suspend_ret_r_delay_cycle * 1/16k */
	unsigned short  deep_xtal_delay_cycle ;			/**< hardware delay time ,deep_ret_xtal_delay_us = deep_xtal_delay_cycle * 1/16k */
	unsigned short  suspend_ret_xtal_delay_cycle ;	/**< hardware delay time ,suspend_ret_xtal_delay_us = suspend_ret_xtal_delay_cycle * 1/16k */
}pm_r_delay_cycle_s;

extern volatile pm_r_delay_cycle_s g_pm_r_delay_cycle;
/**
 * @brief   deep sleep wakeup status
 */
typedef struct{
	unsigned char is_pad_wakeup;
	unsigned char wakeup_src;	//The pad pin occasionally wakes up abnormally in A0. The core wakeup flag will be incorrectly set in A0.
	unsigned char mcu_status;
	unsigned char rsvd;
}pm_status_info_s;

extern _attribute_aligned_(4) pm_status_info_s g_pm_status_info;
extern _attribute_data_retention_sec_ unsigned char g_pm_vbat_v;


/**
 * @brief		This function serves to get deep retention flag.
 * @return		1 deep retention, 0 deep.
 */
static inline unsigned char pm_get_deep_retention_flag(void)
{
	return !(analog_read_reg8(0x7f) & BIT(0));
}

/**
 * @brief		This function serves to get wakeup source.
 * @return		wakeup source.
 */
static _always_inline pm_wakeup_status_e pm_get_wakeup_src(void)
{
	return (pm_wakeup_status_e)analog_read_reg8(0x64);
}

/**
 * @brief		This function serves to clear the wakeup bit.
 * @param[in]	status	- the interrupt status that needs to be cleared.
 * @return		none.
 */
static inline void pm_clr_irq_status(pm_wakeup_status_e status)
{
	analog_write_reg8(0x64, status);/*add by weihua.zhang, confirmed by jianzhi.chen*/
}

/**
 * @brief		This function serves to set the wakeup source.
 * @param[in]	wakeup_src	- wake up source select.
 * @return		none.
 */
static inline void pm_set_wakeup_src(pm_sleep_wakeup_src_e wakeup_src)
{
	analog_write_reg8(0x4b, wakeup_src);
}

/**
 * @brief		This function configures a GPIO pin as the wakeup pin.
 * @param[in]	pin	- the pins can be set to all GPIO except PB0/PC5 and GPIOG groups.
 * @param[in]	pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup).
 * @param[in]	en  - enable or disable the wakeup function for the pan pin(1: enable, 0: disable).
 * @return		none.
 */
void pm_set_gpio_wakeup (gpio_pin_e pin, pm_gpio_wakeup_level_e pol, int en);

/**
 * @brief		This function configures pm wakeup time parameter.
 * @param[in]	param - deep/suspend/deep_retention r_delay time.(default value: suspend/deep_ret=3, deep=11)
 * @return		none.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param);

/**
 * @brief		This function is used in applications where the crystal oscillator is relatively slow to start.
 * 				When the start-up time is very slow, you can call this function to avoid restarting caused
 * 				by insufficient crystal oscillator time (it is recommended to leave a certain margin when setting).
 * @param[in]	delay_us - This time setting is related to the parameter nopnum, which is about the execution time of the for loop
 * 							in the ramcode(default value: 200).
 * @param[in]	loopnum - The time for the crystal oscillator to stabilize is approximately: loopnum*40us(default value: 10).
 * @param[in]	nopnum - The number of for loops used to wait for the crystal oscillator to stabilize after suspend wakes up.
 * 						 for(i = 0; i < nopnum; i++){ asm("tnop"); }(default value: Flash=250).
 * @return		none.
 */
void pm_set_xtal_stable_timer_param(unsigned int delay_us, unsigned int loopnum, unsigned int nopnum);
/**
 * @brief		this function servers to wait bbpll clock lock.
 * @return		none.
 */
_attribute_ram_code_sec_optimize_o2_ void pm_wait_bbpll_done(void);

/**
 * @brief		This function serves to recover system timer.
 * 				The code is placed in the ram code section, in order to shorten the time.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void pm_stimer_recover(void);

#if 0
/**
 * @brief		This function serves to set the working mode of MCU based on 32k crystal,e.g. suspend mode, deep sleep mode, deep sleep with SRAM retention mode and shutdown mode.
 * @param[in]	sleep_mode 			- sleep mode type select.
 * @param[in]	wakeup_src 			- wake up source select.
 * @param[in]	wakeup_tick_type	- tick type select. Use 32K tick count for long-time sleep or 24M tick count for short-time sleep.
 * @param[in]	wakeup_tick			- The tick value at the time of wake-up.
									  If the wakeup_tick_type is PM_TICK_STIMER, then wakeup_tick is converted to 24M. The range of tick that can be set is approximately:
									  current tick value + (18352~0xe0000000), and the corresponding sleep time is approximately: 2ms~234.88s.It cannot go to sleep normally when it exceeds this range.
									  If the wakeup_tick_type is PM_TICK_32K, then wakeup_tick is converted to 32K. The range of tick that can be set is approximately:
									  64~0xffffffff, and the corresponding sleep time is approximately: 2ms~37hours.It cannot go to sleep normally when it exceeds this range.
 * @return		indicate whether the cpu is wake up successful.
 */
_attribute_text_sec_ int pm_sleep_wakeup(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);
#endif
/**
 * @brief		This function serves to set baseband/usb/npe power on/off before suspend sleep,If power
 * 				on this module,the suspend current will increase;power down this module will save current,
 * 				but you need to re-init this module after suspend wakeup.All module is power down default
 * 				to save current.
 * @param[in]	value - whether to power on/off the baseband/usb/npe.
 * @param[in]	on_off - select power on or off, 0 - power off; other value - power on.
 * @return		none.
 */
void pm_set_suspend_power_cfg(pm_suspend_power_cfg_e value, unsigned char on_off);

/**
 * @brief   	This function is used to determine the stability of the crystal oscillator.
 * 				To judge the stability of the crystal oscillator, xo_ready_ana is invalid, and use an alternative solution to judge.
 * 				Alternative principle: Because the clock source of the stimer is the crystal oscillator,
 * 				if the crystal oscillator does not vibrate, the tick value of the stimer does not increase or increases very slowly (when there is interference).
 * 				So first use 24M RC to run the program and wait for a fixed time, calculate the number of ticks that the stimer should increase during this time,
 * 				and then read the tick value actually increased by the stimer.
 * 				When it reaches 50% of the calculated value, it proves that the crystal oscillator has started.
 * 				If it is not reached for a long time, the system will reboot.
 * @param[in]	all_ramcode_en	- Whether all processing in this function is required to be ram code. If this parameter is set to 1, it requires that:
 * 				before calling this function, you have done the disable BTB, disable interrupt, mspi_stop_xip and other operations as the corresponding function configured to 0.
 * @attention   This function can only be called with the 24M clock configuration
 * @return  	none.
 */
_attribute_ram_code_sec_optimize_o2_ void pm_wait_xtal_ready(unsigned char all_ramcode_en);

/**
 * @brief		This function serves to get reboot status.
 * @return		reboot enum element of pm_reboot_event_e.
 * @note        -# if return HW_TIMER_WATCHDOG_REBOOT, need call wd_clear_status() to avoid affecting the next detection of the mcu status;
 *              -# if return HW_32K_WATCHDOG_REBOOT,need call wd_32k_clear_status() to avoid affecting the next detection of the mcu status;
 *              -# if return HW_VBUS_DETECT_REBOOT,need to write 1 and 0 for 0x64(bit7) to avoid affecting the next detection of the mcu status;
 *              -# the interface sys_init() must be called before this interface can be invoked;
 */
pm_reboot_event_e pm_get_reboot_event(void);

/**
 * @brief		this function serves to clear all irq status.
 * @return		Indicates whether clearing irq status was successful.
 */
_attribute_ram_code_sec_optimize_o2_ unsigned char pm_clr_all_irq_status(void);

/**
 * @brief 		This function serves to reboot system.
 * @return 		none 
 */
_always_inline void sys_reset_all(void)
{
	reg_pwdn_en = 0x20;
}

/**
 * @brief		This function serves to switch digital module power. 	
 * @param[in]	module - digital module
 * @param[in]	power_sel - power up or power down
 * @return		none
 * @note		After set digital module power switch register, it need some time to take effect.
 * 				Call the pm_wait_dig_module_power_stable() to check the power switch finish and become stable. 
 * 				The recommendation sequence of enable a module: power up module -> wait power stable -> clock enable.
 */
void pm_set_dig_module_power_switch(pm_dig_module_e module, pm_power_sel_e power_sel);

/**
 * @brief		This function serves to wait digital module stable.  
 * @param[in]	module - digital module
 * @return		none
 */
void pm_wait_dig_module_power_stable(void);

/********************************************************************************************************
 *											internal
 *******************************************************************************************************/

/********************************************************************************************************
 * 				This is just for internal debug purpose, users are prohibited from calling.
 *******************************************************************************************************/

/**
 * @brief 		This function serves to trim dig ldo voltage
 * @param[in]	dig_ldo_trim - dig ldo trim voltage
 * @return		none
 */
void pm_set_dig_ldo_voltage(pm_dig_ldo_trim_e dig_ldo_trim);

/**
 * @brief 		This function serves to trim spd ldo voltage
 * @param[in]	spd_ldo_trim - spd ldo trim voltage
 * @return		none
 */
void pm_set_spd_ldo_voltage(pm_spd_ldo_trim_e spd_ldo_trim);
