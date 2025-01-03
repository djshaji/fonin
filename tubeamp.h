/*
 * GNUitar
 * Distortion effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 */

#ifndef _TUBEAMP_H_
#define _TUBEAMP_H_ 1

#include <lv2.h>
#include <stdbool.h>
#include <stdint.h>
#include "biquad.h"
#include "common.h"
#include "log.h"

#define MAX_STAGES 4

typedef struct tubeamp_params {
    /* internal state variables */
    Biquad_t    highpass[MAX_STAGES];
    Biquad_t    lowpass[MAX_STAGES];
    Biquad_t    biaslowpass[MAX_STAGES];
    Biquad_t    bq_bass, bq_middle, bq_treble;
    Biquad_t    decimation_filter;

    /* user parameters */
    float         * stages, * impulse_model, * impulse_quality;
    float       *gain, * asymmetry, * biasfactor;
    float       * tone_bass, * tone_middle, * tone_treble;
    
    /* internal stuff */
    float       bias[MAX_STAGES];
    float       r_i[MAX_STAGES], r_k_p[MAX_STAGES];
    
    /* effect state stuff */
    float       in[MAX_CHANNELS];
    /* convolution buffer */
    DSP_SAMPLE  *buf[MAX_CHANNELS];
    int_fast16_t    bufidx[MAX_CHANNELS];
    
    /* Shaji */
    float * input, * output ;
    int sample_rate ;
    
    data_block_t db ;
} TubeAmp;


typedef enum {
	INPUT,
	OUTPUT,
	STAGES,
	IMPULSE_QUALITY,
	IMPULSE_MODEL,
	BIAS_FACTOR, // "dynamic" bias
	ASYMMETRY, // "absolute" bias
	GAIN,
	TONE_BASS,
	TONE_MIDDLE,
	TONE_TREBLE
} PortIndex ;

static float
F_tube(float in, float r_i) ;


#endif
