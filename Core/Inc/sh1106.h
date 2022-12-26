/*
 * sh1106.h
 *
 *  Created on: 24 kwi 2022
 *      Author: wolek
 *
 */

#ifndef COMPONENTS_OLED_RTOS_SH1106_SH1106_H_
#define COMPONENTS_OLED_RTOS_SH1106_SH1106_H_

#include "main.h"

//=========================================================

extern void SH1106_HwInit(I2C_HandleTypeDef *i2c);
extern void SH1106_WriteArea(uint8_t X1, uint8_t X2, uint8_t Y1, uint8_t Y2, uint8_t *Buff);
extern void SH1106_SendAll(uint8_t *Buff);
extern void SH1106_WritePageNoBlock(uint8_t Page, uint8_t XStart, uint8_t Len, uint8_t *Buff);
extern void SH1106_SetContrast(uint8_t Contrast);

/*********************************************************************************/
					             // CONFIGURATION //
/*********************************************************************************/

#define MY_DISP_HOR_RES 							128
#define MY_DISP_VER_RES    							64
#define LV_VER_RES_MAX								132

#define SH1106_ADR 									0x78	// i2c address of sh1106 controller
#define DEF_CONTRAST								128		// default contrast

//  better dont change these:

#define DEF_MUX_RATION								0x3F 	// (64)
#define DEF_DISP_OFFSET								0x00	// none
#define DEF_OSC_FREQ								0x50 	// 1/none
#define DEF_PD_CHARGE								0x22 	// 2 DCLKs
#define DEF_VCOM_LEVEL								0x35 	// 0,770



/*********************************************************************************/
					// Control bytes in I2C write mode //
/*********************************************************************************/

#define SH1106_CMD 									0x80	//C0 = 1, D/C = 0
#define SH1106_DATA									0xC0	//C0 = 1, D/C = 1
#define SH1106_MULTI_CMD							0x00	//C0 = 0, D/C = 0
#define SH1106_MULTI_DATA							0x40	//C0 = 0, D/C = 1



/*********************************************************************************/
								// COMMANDS //
/*********************************************************************************/

#define DISP_OFF									0xA4	// turns off entire display
#define DISP_ON_EN									0xA5	// turns on entire display

#define CHPUMP_64V									0x30	// charger pump voltages
#define CHPUMP_74V									0x31
#define CHPUMP_8V									0x32
#define CHPUMP_9V									0x33

#define START_LINE									0x40	// Display start line = 0, to change make | operation
#define SET_CONTRAST								0x81	// Set contras command

#define REMAP_NORMAL								0xA0	// Segment remap modes
#define REMAP_REVERSE								0xA1

#define NORMAL_MODE									0xA6	// display normal/reverse mode
#define REVERSE_MODE								0xA7

#define MUX_RATION									0xA8	// Muultiplex ration mode set (1 - 64)

#define NO_VERT_FLIP								0xC0	// Common output scan direction set
#define VERT_FLIP									0xC8

#define DISP_OFFSET									0xD3	// Set Display Offset

#define OSC_FREQ									0xD5	// Set Oscilator ratio/divider

#define PD_CHARGE									0xD9	// Set precharge/discharge period

#define COMM_PADS_SET								0xDA	// Seto common pads hw configuration
#define COMM_PADS_SEQ								0x02
#define COMM_PADS_ALT								0x12

#define VCOM_LEVEL									0xDB	// Set VCOM deeselect level

#define DCDC_SET									0xAD	// control of DC-DC converter (fist use set and then on/off)
#define DCDC_OFF									0x8A
#define DCDC_ON										0x8B

#define DISP_SLEEP									0xAE	// enter sleep mode
#define DISP_ON										0xAF	// display on


#define SET_L_COL_ADDR 								0x00
#define SET_H_COL_ADDR 								0x10
#define SET_PAGE_ADDR								0xB0



/*********************************************************************************/
									// DNT //
/*********************************************************************************/

#define SH1106_OFFSET								2

#endif /* COMPONENTS_OLED_RTOS_SH1106_SH1106_H_ */
