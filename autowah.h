/*
 * GNUitar
 * Autowah effect: definitions
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

#ifndef _AUTOWAH_H_
#define _AUTOWAH_H_ 1

#include <lv2.h>
#include "backbuf.h"
#include "biquad.h"
#include "log.h"
#include "utils.h"

#define URI "http://shaji.in/plugins/fonin-autowah"

typedef enum {
    INPUT,
    OUTPUT,
    CONTROL,
    PERIOD,
    LAST_FREQ,
    FIRST_FREQ,
    WAH_TYPE,
    RESONANCE,
    DRY_WET,
} PortIndex ;

typedef struct autowah_params {
    Backbuf_t       *history;

    float           * sweep_time; // period
    float           * freq_low; // last freq
    float           * freq_high; // first freq
    float           * res;
    float 	    * drywet;
    float             * sync; // wah control
    float             * method; // wah type

    float           fresh_accum_delta;
    float           fresh_accum_power;
    float           delayed_accum_delta;
    float           delayed_accum_power;
    int             accum_n;
    
    float           f, smoothed_f, freq_vibrato;
    int             dir;
    float           ya[MAX_CHANNELS];
    float           yb[MAX_CHANNELS];
    float           yc[MAX_CHANNELS];
    float           yd[MAX_CHANNELS];

    Biquad_t        lpf;
    Biquad_t        bpf;
    
    int sample_rate ;
    float * input, * output ;
    data_block_t db ;
} AutoWah;

#endif
