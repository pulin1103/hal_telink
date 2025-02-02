/********************************************************************************************************
 * @file    clock.h
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
/**	@page CLOCK
 *
 *	Introduction
 *	===============
 *	B92 clock setting.
 *
 *	API Reference
 *	===============
 *	Header File: clock.h
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "compiler.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 *  @note	If it is an external flash, the maximum speed of mspi needs to be based on the board test.
 *  		Because the maximum speed is related to the wiring of the board, and is also affected by temperature and GPIO voltage,
 *  		the maximum speed needs to be tested at the highest and lowest voltage of the board,
 *  		and the high and low temperature long-term stability test speed is no problem.
 */
#define PLL_240M_CCLK_120M_HCLK_60M_PCLK_60M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK, HCLK_DIV1_TO_PCLK, CLK_DIV5)
#define PLL_240M_CCLK_120M_HCLK_60M_PCLK_30M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, CLK_DIV5)

#define PLL_240M_CCLK_80M_HCLK_40M_PCLK_40M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV3, CCLK_DIV2_TO_HCLK, HCLK_DIV1_TO_PCLK, CLK_DIV5)
#define PLL_240M_CCLK_80M_HCLK_40M_PCLK_20M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV3, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, CLK_DIV5)

#define PLL_240M_CCLK_60M_HCLK_60M_PCLK_60M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV4, CCLK_DIV1_TO_HCLK, HCLK_DIV1_TO_PCLK, CLK_DIV5)
#define PLL_240M_CCLK_60M_HCLK_60M_PCLK_30M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV4, CCLK_DIV1_TO_HCLK, HCLK_DIV2_TO_PCLK, CLK_DIV5)
#define PLL_240M_CCLK_60M_HCLK_60M_PCLK_15M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV4, CCLK_DIV1_TO_HCLK, HCLK_DIV4_TO_PCLK, CLK_DIV5)

#define PLL_240M_CCLK_60M_HCLK_30M_PCLK_30M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV4, CCLK_DIV2_TO_HCLK, HCLK_DIV1_TO_PCLK, CLK_DIV5)
#define PLL_240M_CCLK_60M_HCLK_30M_PCLK_15M_MSPI_48M			clock_init(BASEBAND_PLL, CLK_DIV4, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, CLK_DIV5)

#define PLL_240M_CCLK_120M_HCLK_60M_PCLK_60M_MSPI_40M			clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK, HCLK_DIV1_TO_PCLK, CLK_DIV6)
#define PLL_240M_CCLK_120M_HCLK_60M_PCLK_30M_MSPI_40M			clock_init(BASEBAND_PLL, CLK_DIV2, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, CLK_DIV6)
/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 *  @brief  Define sys_clk struct.
 */
typedef struct {
	unsigned short pll_clk;		/**< pll clk */
	unsigned char cclk;			/**< cpu clk */
	unsigned char hclk;			/**< hclk */
	unsigned char pclk;			/**< pclk */
	unsigned char mspi_clk;		/**< mspi_clk */
}sys_clk_t;

/**
 *  @brief  Define sys_clk_config_t struct.
 */
typedef struct {
	unsigned char cclk_cfg;			/* cpu clk cfg */
	unsigned char hclk_cfg;			/* hclk cfg */
	unsigned char pclk_cfg;			/* pclk cfg */

	unsigned char mspi_clk_cfg;		/* mspi_clk cfg */

	unsigned char is_saved;		/* is saved */
}sys_clk_config_t;

/**
 * @brief system clock type
 * |                                     |                                    |               |
 * | :-----------------------------------| :--------------------------------- | :------------ |
 * |             <4:0>                   |                 <7:5>              |    <15:8>     |
 * | analog_86<4:0> bbpll_240M_div_ratio | analog_84<7:5> bbpll_240M_vco_itrim|      clk      |
 */
typedef enum{
	PLL_CLK_192M 	= (0x10 | (7 << 5) | (192 << 8)),
	PLL_CLK_240M 	= (0x12 | (5 << 5) | (240 << 8)),
	PLL_CLK_288M 	= (0x14 | (1 << 5) | (288 << 8)),
}sys_pll_clk_e;

/**
 * @brief system clock type.
 */
typedef enum{
	RC_24M			= 0x00,
	XTAL_24M		= 0x10,
	BASEBAND_PLL	= 0x20,
}sys_clock_src_e;

/**
 * @brief 32K clock type.
 */
typedef enum{
	CLK_32K_RC   =0,
	CLK_32K_XTAL =1,
}clk_32k_type_e;

/**
 * @brief clock div type.
 */
typedef enum{
	CLK_DIV1 = 1,
	CLK_DIV2,
	CLK_DIV3,
	CLK_DIV4,
	CLK_DIV5,
	CLK_DIV6,
	CLK_DIV7,
	CLK_DIV8,
	CLK_DIV9,
	CLK_DIV10,
	CLK_DIV11,
	CLK_DIV12,
	CLK_DIV13,
	CLK_DIV14,
	CLK_DIV15,
}sys_clock_div_e;

/**
 * @brief cclk div to hclk.
 */
typedef enum{
	CCLK_DIV1_TO_HCLK    =    0,
	CCLK_DIV2_TO_HCLK    =    1,
}sys_cclk_div_to_hclk_e;

/**
 * @brief hclk div to pclk.
 */
typedef enum{
	HCLK_DIV1_TO_PCLK    =    0,
	HCLK_DIV2_TO_PCLK    =    1,
	HCLK_DIV4_TO_PCLK    =    2,
}sys_hclk_div_to_pclk_e;

/**
 *  @brief  Define rc_24M_cal enable/disable.
 */
typedef enum {
	RC_24M_CAL_DISABLE=0,
	RC_24M_CAL_ENABLE,
}rc_24M_cal_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern sys_clk_t sys_clk;
extern clk_32k_type_e g_clk_32k_src;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief       This function use to select the system clock source.
 * @param[in]	src - cclk source.
 * @param[in]	cclk_div - the cclk divide from src.
 * @param[in]	hclk_div - the hclk divide from cclk.
 * @param[in]	pclk_div - the pclk divide from hclk.
 * @param[in]	mspi_clk_div - mspi_clk has two source - pll div and 24M rc. If it is built-in flash, the maximum speed of mspi is 64M.
							   If it is an external flash, the maximum speed of mspi needs to be based on the board test.
							   Because the maximum speed is related to the wiring of the board, and is also affected by temperature and GPIO voltage,
							   the maximum speed needs to be tested at the highest and lowest voltage of the board,
							   and the high and low temperature long-term stability test speed is no problem.
 * @return      none
 * @note		Do not switch the clock during the DMA sending and receiving process;
 * 			    because during the clock switching process, the system clock will be
 * 			    suspended for a period of time, which may cause data loss
 */
_attribute_ram_code_sec_noinline_
void clock_init(sys_clock_src_e src,
				sys_clock_div_e cclk_div,
				sys_cclk_div_to_hclk_e hclk_div,
				sys_hclk_div_to_pclk_e pclk_div,
				sys_clock_div_e mspi_clk_div);

/**
 * @brief   	This function serves to set 32k clock source.
 * @param[in]   src - variable of 32k type.
 * @return  	none.
 */
void clock_32k_init(clk_32k_type_e src);

/**
 * @brief   	This function serves to kick 32k xtal.
 * @param[in]   xtal_times - kick times.
 * @return  	1 success, 0 error.
 */
unsigned char clock_kick_32k_xtal(unsigned char xtal_times);

/**
 * @brief     	This function performs to select 24M as the system clock source.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void clock_cal_24m_rc (void);

/**
 * @brief     This function performs to select 32K as the system clock source.
 * @return    none.
 */
void clock_cal_32k_rc (void);

/**
 * @brief  This function serves to get the 32k tick.
 * @return none.
 */
_attribute_ram_code_sec_noinline_  unsigned int clock_get_32k_tick (void);

/**
 * @brief  This function serves to set the 32k tick.
 * @param  tick - the value of to be set to 32k.
 * @return none.
 */
_attribute_ram_code_sec_noinline_ void clock_set_32k_tick(unsigned int tick);

/**
 * @brief       This function use to configure the mspi clock source.
 * @param[in]	src - the mspi clk source
 * @param[in]	div - the mspi clk source divider
 * @return 		none.
 */
void clock_mspi_clk_config(sys_clock_src_e src, sys_clock_div_e div);

/**
 * @brief       This function used to configure the frequency of CCLK/HCLK/PCLK when the PLL is 240M.
 * 				You need to wait until all the peripherals that use these clocks are idle before you can switch frequencies.
 * @param[in]   src - clock source.
 * @param[in]   cclk_div - divider of CCLK.
 * @param[in]   hclk_div - divider of HCLK.
 * @param[in]   pclk_div - divider of PCLK.
 * @return      none
 */
void clock_cclk_hclk_pclk_config(sys_clock_src_e src, sys_clock_div_e cclk_div,
								sys_cclk_div_to_hclk_e hclk_div,
								sys_hclk_div_to_pclk_e pclk_div);

/**
 * @brief		This function use to set all clock to default. 
 * @return		none.
 * @note		After call this, the following clock will set to default source and value:
 * 				-----------------------------------------------------------------------
 * 				clock source |			clock
 * 				-----------------------------------------------------------------------
 * 				RC 24M		 | CCLK 24M, HCLK 24M, PCLK 24M, MSPI CLK 24M.
 * 				-----------------------------------------------------------------------
 */
void clock_set_all_clock_to_default(void);

/**
 * @brief		This function use to save all clock configuration for the follow-up restore. 
 * @return		none.
 * @note		This function needs to be used in conjunction with clock_restore_clock_config().
 */
void clock_save_clock_config(void);

/**
 * @brief		This function use to restore all previously saved clock configurations.
 * @return		none.
 * @note		This function needs to be used in conjunction with clock_save_clock_config().
 */
void clock_restore_clock_config(void);
#endif

