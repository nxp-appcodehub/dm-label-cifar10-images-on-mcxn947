 /*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 // #include "board_init.h"

#include <stdio.h>
#include "fsl_debug_console.h"
#include "image.h"
#include "image_utils.h"
#include "model.h"
#include "output_postproc.h"
#include "timer.h"
#include "video.h"

extern "C" {

#define MODEL_IN_W	128
#define MODEL_IN_H  128
#define MODEL_IN_C	3
#define MODEL_IN_COLOR_BGR 0


__attribute__((section (".model_input_buffer"))) static uint8_t model_input_buf[MODEL_IN_W*MODEL_IN_H*MODEL_IN_C] = {0};

uint32_t s_infUs = 0;
volatile uint8_t g_isImgBufReady = 0;
#define WND_X0 4
#define WND_Y0 4

void Rgb565StridedToBgr888(const uint16_t* pIn, int srcW, int wndW, int wndH, int wndX0, int wndY0,
	uint8_t* p888, int stride, uint8_t isSub128) {
	const uint16_t* pSrc = pIn;
	uint32_t datIn, datOut, datOuts[3];
	uint8_t* p888out = p888;
	for (int y = wndY0,y1=(wndH-wndY0)/stride-wndY0; y < wndH; y += stride,y1--) {
		pSrc = pIn + srcW * y + wndX0;
		//p888out = p888 + y1*wndW*3/stride;
		for (int x = 0; x < wndW; x += stride * 4) {
			datIn = pSrc[0];
			pSrc += stride;
			datOuts[0] = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			// datOuts[0] = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);

			datIn = pSrc[0];
			pSrc += stride;
			datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			// datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);
			datOuts[0] |= datOut << 24;
			datOuts[1] = datOut >> 8;

			datIn = pSrc[0];
			pSrc += stride;
			datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			// datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);
			datOuts[1] |= (datOut << 16) & 0xFFFF0000;
			datOuts[2] = datOut >> 16;

			datIn = pSrc[0];
			pSrc += stride;
			datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			// datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);

			datOuts[2] |= datOut << 8;

			if (isSub128) {
				// subtract 128 bytewisely, equal to XOR with 0x80
				datOuts[0] ^= 0x80808080;
				datOuts[1] ^= 0x80808080;
				datOuts[2] ^= 0x80808080;
			}
			memcpy(p888out, datOuts, 3 * 4);
			p888out += 3 * 4;
		}
	}
}

void Rgb565StridedToRgb888(const uint16_t* pIn, int srcW, int wndW, int wndH, int wndX0, int wndY0,
	uint8_t* p888, int stride, uint8_t isSub128) {
	const uint16_t* pSrc = pIn;
	uint32_t datIn, datOut, datOuts[3];
	uint8_t* p888out = p888;

	for (int y = wndY0,y1=(wndH-wndY0)/stride-wndY0; y < wndH; y += stride,y1--) {
		pSrc = pIn + srcW * y + wndX0;

		//p888out = p888 + y1*wndW*3/stride;
		for (int x = 0; x < wndW; x += stride * 4) {
			datIn = pSrc[0];
			pSrc += stride;
			// datOuts[0] = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			datOuts[0] = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);

			datIn = pSrc[0];
			pSrc += stride;
			// datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);
			datOuts[0] |= datOut << 24;
			datOuts[1] = datOut >> 8;

			datIn = pSrc[0];
			pSrc += stride;
			// datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);
			datOuts[1] |= (datOut << 16) & 0xFFFF0000;
			datOuts[2] = datOut >> 16;

			datIn = pSrc[0];
			pSrc += stride;
			// datOut = (datIn & 31) << 3 | (datIn & 63 << 5) << 5 | (datIn & 31 << 11) << 8;
			datOut = (datIn & 31) << 19| (datIn & 63 << 5) << 5 | ((datIn>>8) & 0xf8);

			datOuts[2] |= datOut << 8;

			if (isSub128) {
				// subtract 128 bytewisely, equal to XOR with 0x80
				datOuts[0] ^= 0x80808080;
				datOuts[1] ^= 0x80808080;
				datOuts[2] ^= 0x80808080;
			}
			memcpy(p888out, datOuts, 3 * 4);
			p888out += 3 * 4;
		}
	}
}

void ezh_copy_slice_to_model_input(uint32_t idx, uint32_t cam_slice_buffer, uint32_t cam_slice_width, uint32_t cam_slice_height, uint32_t max_idx)
{
	static uint8_t* pCurDat;
	uint32_t curY;
	uint32_t s_imgStride = cam_slice_width / MODEL_IN_W;


	if (idx > max_idx)
		return;
	//uint32_t ndx = max_idx -1 - idx;
	uint32_t ndx = idx;
	curY = ndx * cam_slice_height;
	int wndY = (s_imgStride - (curY - WND_Y0) % s_imgStride) % s_imgStride;


	if (idx +1 >= max_idx)
		g_isImgBufReady = 1;

	pCurDat = model_input_buf + 3 * ((cam_slice_height * ndx + wndY) * cam_slice_width / s_imgStride / s_imgStride);

	if (curY + cam_slice_height >= WND_Y0){

		if (MODEL_IN_COLOR_BGR == 1) {
			Rgb565StridedToBgr888((uint16_t*)cam_slice_buffer, cam_slice_width, cam_slice_width, cam_slice_height, WND_X0, wndY, pCurDat, s_imgStride, 1);
		}else {
			Rgb565StridedToRgb888((uint16_t*)cam_slice_buffer, cam_slice_width, cam_slice_width, cam_slice_height, WND_X0, wndY, pCurDat, s_imgStride, 1);
		}
	}
}


void cifar10_recognize()
{
	tensor_dims_t inputDims;
	tensor_type_t inputType;
	uint8_t* inputData;

	tensor_dims_t outputDims;
	tensor_type_t outputType;
	uint8_t* outputData;
	size_t arenaSize;

	if (MODEL_Init() != kStatus_Success)
	{
		PRINTF("Failed initializing model");
		for (;;) {}
	}

	size_t usedSize = MODEL_GetArenaUsedBytes(&arenaSize);
	PRINTF("\r\n%d/%d kB (%0.2f%%) tensor arena used\r\n", usedSize / 1024, arenaSize / 1024, 100.0*usedSize/arenaSize);

	inputData = MODEL_GetInputTensorData(&inputDims, &inputType);
	outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);

	while(1)
	{
		if (g_isImgBufReady == 0)
			continue;

		uint8_t *buf = 0;

		memset(inputData,0,inputDims.data[1]*inputDims.data[2]*inputDims.data[3]);
		buf = inputData + (inputData,inputDims.data[1] - MODEL_IN_H) /2 * MODEL_IN_W * MODEL_IN_C;
		memcpy(buf, model_input_buf, MODEL_IN_W*MODEL_IN_H*MODEL_IN_C);

		auto startTime = TIMER_GetTimeInUS();
		MODEL_RunInference();
		auto endTime = TIMER_GetTimeInUS();

		auto dt = endTime - startTime;
		s_infUs = (uint32_t)dt;

		MODEL_ProcessOutput(outputData, &outputDims, outputType, dt);


	}
}


}


