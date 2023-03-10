/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>


#include "main.h"
#include "sh1106.h"

/*********************
 *      DEFINES
 *********************/

#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))


#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(I2C_HandleTypeDef *i2c);

#if USE_DMA == 0
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
#else
static void disp_flush_dma(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static void dma_send_routine(void);
#endif
static void set_px_cb(struct _lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        				lv_color_t color, lv_opa_t opa);
static void rounder_cb(struct _lv_disp_drv_t * disp_drv, lv_area_t * area);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/* those variables keep temporary values of data to send in non-blocking mode
 * PageStart - number of page whhere "flushing" data starts of
 * PagesToSend - how many pages have to be flushed (summary)
 * PagesLeftToSend - how many pages left
 * XPositionStart - which column data starts of
 * XPixels - how many pixels within one page we want to flush
 *
 * and two helpful pointers */
#if USE_DMA
static volatile uint8_t PageStart, PagesToSend, PagesLeftToSend, XPositionStart, XPixels;
static lv_disp_drv_t *DispDrv;
static uint8_t *BuffPointer;
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_DispInit(I2C_HandleTypeDef *I2C){
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init(I2C);

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

//    /* Example for 2) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/
//
//    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
#if USE_DMA == 0
    disp_drv.flush_cb = disp_flush;
#else
    disp_drv.flush_cb = disp_flush_dma;
#endif
    disp_drv.set_px_cb = set_px_cb;
    disp_drv.rounder_cb = rounder_cb;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(I2C_HandleTypeDef *i2c){

	SH1106_HwInit(i2c);
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
#if USE_DMA == 0
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    	uint8_t *buff = (uint8_t *)color_p;
    	SH1106_WriteArea(area->x1, area->x2, area->y1, area->y2, buff);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

#else
static void disp_flush_dma(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    	/* fill global variables */
    	PageStart = (area->y1 / 8);
    	uint8_t lastPage = (area->y2 / 8);
    	PagesLeftToSend = lastPage - PageStart + 1;
    	PagesToSend = PagesLeftToSend;

    	BuffPointer = (uint8_t *)color_p;
    	XPositionStart = area->x1;
    	XPixels = area->x2 - area->x1 + 1;

    	DispDrv = disp_drv;

    	/* start the routine where the magic happens :) */
    	dma_send_routine();
    }

    /*IMPORTANT!!!
     *do not inform now that you are ready with flushing because you are not!
     *dma_send_routine() will do it when routine will be finished */
//    lv_disp_flush_ready(disp_drv);
}

#endif

/* two examples taken from internet... */
static void set_px_cb(struct _lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        				lv_color_t color, lv_opa_t opa){

	uint16_t byte_index = x + ((y / 8) * buf_w);
	uint8_t  bit_index  = y & 0x7;

	// == 0 inverts, so we get blue on black
	if(color.full == 0){

		BIT_SET( buf[byte_index], bit_index);
	}
	else{

		BIT_CLEAR( buf[byte_index], bit_index);
	}
}


static void rounder_cb(struct _lv_disp_drv_t * disp_drv, lv_area_t * area){

	area->y1 = (area->y1 & (~0x7));
	area->y2 = ((area->y2 & (~0x7)) + 7);
}


#if USE_DMA
/* this function should be called everytime:
 * - if the last transfer has been finished
 * - if we want to send a new buffer */
static void dma_send_routine(void){

	if(PagesLeftToSend > 0){

		/* if there is still something to send... */
		uint8_t page_shift = PagesToSend - PagesLeftToSend;
		uint8_t temp_page = (PageStart + page_shift);
		SH1106_WritePageNoBlock(temp_page, XPositionStart, XPixels, &BuffPointer[page_shift * XPixels]);
		PagesLeftToSend--;
	}
	else{

		/*IMPORTANT!!
		 * Here inform the graphics library that you are ready with the flushing */
		lv_disp_flush_ready(DispDrv);
	}
}


/* external function for user to call when transfer is complete */
void lv_port_DmaTxComplete(void){

	dma_send_routine();
}
#endif


/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
