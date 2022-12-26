/*
 * sh1106.c
 *
 *  Created on: 24 kwi 2022
 *      Author: wolek
 */

//=========================================================

#include "string.h"
#include "sh1106.h"
#include "main.h"

//=========================================================

#define HORIZONTAL_RES 			MY_DISP_HOR_RES
#define PAGES					(MY_DISP_VER_RES / 8)

#define SH1106_TIMEOUT_MS 		100


/*********************************************************************************/
						// Buffer with initialization data //
/*********************************************************************************/

static const uint8_t init_commands[] = {

		DISP_OFF,					// entire display off
		CHPUMP_8V,					// charger pump voltage - 8V
		START_LINE,					// start line set to 0

		SET_CONTRAST,				// set contrast to default
		DEF_CONTRAST,

//		REMAP_NORMAL,				// normal remap mode
		REMAP_REVERSE,
		NORMAL_MODE,				// normal display mode

		MUX_RATION,					// set multiplex ration mode to default
		DEF_MUX_RATION,

//		NO_VERT_FLIP,				// normaln commmon output scan direction
		VERT_FLIP,

		DISP_OFFSET,				// set display offset to default
		DEF_DISP_OFFSET,

		OSC_FREQ,					// set oscilator parameters to default
		DEF_OSC_FREQ,

		PD_CHARGE,					// set precharge/discharge periods to default
		DEF_PD_CHARGE,

		COMM_PADS_SET,				// seto common pads hw config to alternative
		COMM_PADS_ALT,

		VCOM_LEVEL,					// set VCOM deselect level to default
		DEF_VCOM_LEVEL,

		DCDC_SET,					// turn on internal dc-dc converter
		DCDC_ON,

		DISP_ON,					// turns on the display

};



static I2C_HandleTypeDef *sh1106_i2c;



/*********************************************************************************/
							// Low level I2C functions //
/*********************************************************************************/


static void sh1106_cmd(uint8_t cmd){

//	i2c_write_byte(SH1106_ADR, SH1106_CMD, cmd, SH1106_I2C_NR);
	HAL_I2C_Mem_Write(sh1106_i2c, SH1106_ADR, SH1106_CMD, 1, &cmd, 1, SH1106_TIMEOUT_MS);
}

static void sh1106_multi_cmd(uint8_t *cmd, uint8_t len){

//	i2c_write_buff(SH1106_ADR, SH1106_MULTI_CMD, cmd, len, SH1106_I2C_NR);
	HAL_I2C_Mem_Write(sh1106_i2c, SH1106_ADR, SH1106_MULTI_CMD, 1, cmd, len, SH1106_TIMEOUT_MS);
}

static void sh1106_multi_data(uint8_t *data, uint8_t len){

//	i2c_write_buff(SH1106_ADR, SH1106_MULTI_DATA, data, len, SH1106_I2C_NR);
	HAL_I2C_Mem_Write(sh1106_i2c, SH1106_ADR, SH1106_MULTI_DATA, 1, data, len, SH1106_TIMEOUT_MS);
}



/*********************************************************************************/
								// Basic functions //
/*********************************************************************************/

static void sh1106_set_col(uint8_t col){

	if(col > (HORIZONTAL_RES - 1)) return;

	uint8_t buff[2];

	buff[0] = ((col + SH1106_OFFSET) & 0b00001111) | SET_L_COL_ADDR;
	buff[1] = ((col + SH1106_OFFSET) >> 4) | SET_H_COL_ADDR;

	sh1106_multi_cmd(buff, 2);
}

static void sh1106_set_page(uint8_t page){

	if(page > (PAGES - 1)) return;

	uint8_t a = (page & 0b00000111) | SET_PAGE_ADDR;

	sh1106_cmd(a);
}





/*********************************************************************************/

							// oled initialization //

/*********************************************************************************/

void SH1106_HwInit(I2C_HandleTypeDef *i2c){

	sh1106_i2c = i2c;

	uint8_t a = sizeof(init_commands);
	uint8_t tab[a];

	memcpy(tab, init_commands, a);

	sh1106_multi_cmd(tab, a);
}




/*********************************************************************************/

						// Send buffer to oled //
				// Y1 and Y2 have to be (multiplication of 8) -1 //
				// f.e. 0, 7, 15 etc.

/*********************************************************************************/

void SH1106_Send(uint8_t X1, uint8_t X2, uint8_t Y1, uint8_t Y2, uint8_t *Buff){

	uint8_t xPixels = X2 - X1;

	uint8_t firstPage = (Y1 / 8);
	uint8_t lastPage = (Y2 / 8);
	uint8_t pages = lastPage - firstPage + 1;

	for(uint8_t a = 0; a < pages; a++){

		sh1106_set_page(a + firstPage);
		sh1106_set_col(X1);
		sh1106_multi_data(&Buff[a * xPixels], xPixels);
	}
}

/*********************************************************************************/

								// Set whole buffer to oled //

/*********************************************************************************/

void SH1106_SendAll(uint8_t *Buff){

	for(uint8_t a = 0; a < PAGES; a++){

		sh1106_set_page(a);
		sh1106_set_col(0);
		sh1106_multi_data(&Buff[a * HORIZONTAL_RES], HORIZONTAL_RES);
	}
}

/*********************************************************************************/

								// Set oled contrast //

/*********************************************************************************/

void oled_set_contrast(uint8_t value){

	uint8_t buff[2] = { SET_CONTRAST, value };
	sh1106_multi_cmd(buff, 2);
}

