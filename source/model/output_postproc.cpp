/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "output_postproc.h"
#include "get_top_n.h"
#include "demo_config.h"
#include "labels.h"
#ifdef EIQ_GUI_PRINTF
#include "chgui.h"
#endif
uint32_t g_label_num=0;
uint32_t g_detedted_flag=0;
status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type, int inferenceTime)
{
	const float threshold = (float)DETECTION_TRESHOLD / 100;
	    result_t topResults[NUM_RESULTS];
	    const char* label = "N/A";
	    g_detedted_flag = 0;

	    /* Find best label candidates. */
	    MODEL_GetTopN(data, dims->data[dims->size - 1], type, NUM_RESULTS, threshold, topResults);

	    float confidence = 0;
	    if (topResults[0].index >= 0)
	    {
	        auto result = topResults[0];
	        confidence = result.score;
	        int index = result.index;
	        if (confidence * 100 > DETECTION_TRESHOLD)
	        {
	            label = labels[index];
	            g_label_num = index;
	            g_detedted_flag = 1;
	        }
	    }

	    int score = (int)(confidence * 100);
	#if 0
	    PRINTF("----------------------------------------" EOL);
	    PRINTF("     Inference time: %d ms" EOL, inferenceTime / 1000);
	    PRINTF("     Detected: %s (%d%%)" EOL, label, score);
	    PRINTF("----------------------------------------" EOL);
	#else
	    PRINTF("---%s (%d%%), %d ms---" EOL, label, score, inferenceTime / 1000);


	#endif
	#ifdef EIQ_GUI_PRINTF
	    GUI_PrintfToBuffer(GUI_X_POS, GUI_Y_POS, "Detected: %.20s (%d%%)", label, score);
	#endif

    return kStatus_Success;
}
