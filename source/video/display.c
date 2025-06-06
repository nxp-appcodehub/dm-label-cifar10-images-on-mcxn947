/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_inputmux.h"
#include "clock_config.h"
#include "board.h"
#include "lcd_impl_flexio.h"
#include "st7796_lcd.h"
#include "video.h"
#include "ASCII_consolas_3618.h"
#include "labels.h"
/****************************************************************
LCD display
****************************************************************/
volatile uint16_t g_camera_display_buffer[LCD_BUFF_WIDTH*LCD_BUFF_HEIGHT]__attribute((aligned(32)));

struct  lcd_coordinate{
			uint16_t x_start;
			uint16_t y_start;
			uint16_t x_end;
			uint16_t y_end;
		};


struct lcd_coordinate display_slice_area_320_240_brk[18] = {
     //{ 0, 0, 0,0 },//reserve

     { 0, 40+15*0, 320U-1U, 40+15*1-1 },//320*240/ 16
     { 0, 40+15*1, 320U-1U, 40+15*2-1 },//320*240/ 16
     { 0, 40+15*2, 320U-1U, 40+15*3-1 },//320*240/ 16
     { 0, 40+15*3, 320U-1U, 40+15*4-1 },//320*240/ 16
     { 0, 40+15*4, 320U-1U, 40+15*5-1 },//320*240/ 16
     { 0, 40+15*5, 320U-1U, 40+15*6-1 },//320*240/ 16
     { 0, 40+15*6, 320U-1U, 40+15*7-1 },//320*240/ 16
     { 0, 40+15*7, 320U-1U, 40+15*8-1 },//320*240/ 16
     { 0, 40+15*8, 320U-1U, 40+15*9-1 },//320*240/ 16
     { 0, 40+15*9, 320U-1U, 40+15*10-1 },//320*240/ 16

     { 0, 40+15*10, 320U-1U, 40+15*11-1 },//320*240/ 16
     { 0, 40+15*11, 320U-1U, 40+15*12-1 },//320*240/ 16
     { 0, 40+15*12, 320U-1U, 40+15*13-1 },//320*240/ 16
     { 0, 40+15*13, 320U-1U, 40+15*14-1 },//320*240/ 16
     { 0, 40+15*14, 320U-1U, 40+15*15-1 },//320*240/ 16
     { 0, 40+15*15, 320U-1U, 40+15*16-1 },//320*240/ 16

};

struct lcd_coordinate display_slice_area_320_240_rotate_0[] = {
     //{ 0, 0, 0,0 },//reserve

		{0,LCD_BUFF_HEIGHT*0, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*1-1},
		{0,LCD_BUFF_HEIGHT*1, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*2-1},
		{0,LCD_BUFF_HEIGHT*2, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*3-1},
		{0,LCD_BUFF_HEIGHT*3, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*4-1},
		{0,LCD_BUFF_HEIGHT*4, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*5-1},
		{0,LCD_BUFF_HEIGHT*5, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*6-1},
		{0,LCD_BUFF_HEIGHT*6, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*7-1},
		{0,LCD_BUFF_HEIGHT*7, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*8-1},
		{0,LCD_BUFF_HEIGHT*8, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*9-1},
		{0,LCD_BUFF_HEIGHT*9, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*10-1},
		{0,LCD_BUFF_HEIGHT*10, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*11-1},
		{0,LCD_BUFF_HEIGHT*11, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*12-1},
		{0,LCD_BUFF_HEIGHT*12, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*13-1},
		{0,LCD_BUFF_HEIGHT*13, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*14-1},
		{0,LCD_BUFF_HEIGHT*14, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*15-1},
		{0,LCD_BUFF_HEIGHT*15, LCD_WINDOW_HEIGHT-1,LCD_BUFF_HEIGHT*16-1},

};

struct lcd_coordinate Pic_nxp_ai_demo[5] = {
    {  448U, 16U, 800U-1U, 16+448-1U  },//align edge
     { 224, 48,    480- 1, 272-1 },
	 {224 + 96, 116 - 15, 480-1, 256 - 1 - 15},
     { 0, 240,    320- 1, 480-1 },
	 { 0, 0,    480- 1, 320-1 },
};

static lcd_impl_flexio_t s_lcd_impl;
static st7796_lcd_t s_lcd = {
    .config =
        {
			.direction = ST7796_DIR_0,
			.pix_fmt   = ST7796_RGB565,
			.bgr_mode  = 1,
			.inversion = 0,
			.mirrored  = 0,
			.vflip = 1,
        },
    .cb =
        {
            .reset_cb      = lcd_impl_reset,
            .write_cmd_cb  = lcd_impl_write_cmd,
            .write_data_cb = lcd_impl_write_data,
        },
    .user_data = &s_lcd_impl,
};

uint16_t POINT_COLOR=0x0<<11;
uint16_t BACK_COLOR =0xffff;
__attribute__ ((aligned (32))) volatile uint16_t g_char_buf[32][16] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
void display_show_string(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
void display_window_clear();

__WEAK const char * GetBriefString(void) {

    return 0;
}

void display_init()
{
	lcd_impl_init(&s_lcd_impl);
	st7796_lcd_init(&s_lcd);
	display_window_clear();

	uint8_t mcxn9[] =    "  MCXN94x  ";
	uint8_t aidemo[] =   "  Cifar10  ";
	display_show_string(60U,  250, 200, 32, 32, mcxn9);
	display_show_string(60U,  280, 200, 32, 32, aidemo);

}


void display_font_conversion(uint8_t num,uint8_t size,uint8_t mode){
    uint8_t temp,t1,t,buf_x, buf_y;
    uint8_t csize=(size/8+((size%8)?1:0));
    num=num- 32;
    buf_x = 0;
    buf_y = 0;
    for(uint32_t i=0;i<size/2;i++){
      for(t=0;t<csize;t++)
      {
        temp=asc2_2412[num][csize*i + t];
        for(t1=0;t1<8;t1++)
        {
          if(temp&0x80){
            g_char_buf[buf_y][buf_x] = POINT_COLOR;
          }else if(mode==0){
            g_char_buf[buf_y][buf_x] = BACK_COLOR;
          }
          temp<<=1;
          buf_y++;
         }
      }
      buf_y = 0;
      buf_x++;
    }
}

void display_show_string(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
	 uint16_t xstart = x;
	 uint16_t x0,x1,y0,y1;

	 width+=x;
	 height+=y;
	 while((*p<='~')&&(*p>=' '))
	 {
		 if(x>=width){x=xstart;y+=size;}
		 if(y>height)break;

		 display_font_conversion(*p, size, 0);
		 x0 = x;
		 x1 = x + size/2 - 1;
		 y0 = y;
		 y1 = y + size - 1;

		 st7796_lcd_load(&s_lcd,(uint8_t *)g_char_buf,x0,x1,y0,y1);

		 x+=size/2;
		 p++;
	 }
}

static void convert_384_384_to_192_192(uint16_t *input,uint16_t *output, uint32_t height,uint32_t width)
{
	int16_t m = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((i % 2 == 0 && j % 2 == 0)) {
            	output[m++]	= input[i* 384 + j];
            }
        }
    }
}

extern uint32_t g_label_num;
extern uint32_t g_detedted_flag;
void display_show_slice(uint32_t g_stripe_index, uint32_t buffer, uint32_t max_idx)
{
	convert_384_384_to_192_192(buffer, g_camera_display_buffer, STRIPE_HEIGHT, CAMERA_WIDTH);

    st7796_lcd_load(
		  &s_lcd,
		  (uint8_t *)(g_camera_display_buffer), // data
		  display_slice_area_320_240_rotate_0[g_stripe_index].x_start+64, // startX
		  display_slice_area_320_240_rotate_0[g_stripe_index].x_end+64, // endX
		  display_slice_area_320_240_rotate_0[g_stripe_index].y_start+24, // startY
		  display_slice_area_320_240_rotate_0[g_stripe_index].y_end+24 // endY
		  );

    if(g_stripe_index +1 == max_idx){

		if(g_detedted_flag){
			char str_buf[96] = {0x0};
			sprintf(str_buf,"Label: %s",labels[g_label_num]);
			display_show_string( 32 + 0, 360, 280,32,32, (uint8_t *)str_buf);
		}
		else
		{
			display_show_string( 32 + 0, 360, 280,32,32, (uint8_t *)"                    ");
		}

	 }

}


void display_window_clear()
{
	struct lcd_coordinate win =  { 0, 0,    480- 1, 320-1 };

	memset(g_camera_display_buffer,0xff, LCD_BUFF_WIDTH*LCD_BUFF_HEIGHT*2);
	int buffer_h = (LCD_BUFF_WIDTH*LCD_BUFF_HEIGHT/480);

	for (int i=0;i < 480/buffer_h;i++)
	{

		st7796_lcd_load(
						  &s_lcd,
						  (uint8_t *)(g_camera_display_buffer), // data
						  0, // startX
						  480-1, // endX
						  i*buffer_h, // startY
						  i*buffer_h + buffer_h-1 // endY
		);

	}

}
