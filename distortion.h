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

#ifndef _DISTORT_H_
#define _DISTORT_H_ 1

#include <lv2.h>
#include <stdint.h>
#include "biquad.h"
#include "common.h"

#include "rcfilter.h"

typedef enum {
    INPUT,
    OUTPUT,
    DRIVE,
    LEVEL,
    SATURATION,
    LOWPASS_
} PortIndex ;

typedef struct distort_params {
    float             * sat,
                    * level,
                    * drive,
                    * lowpass ;
                    
    int lastval[MAX_CHANNELS],
                    noisegate;
    struct filter_data fd;
    struct filter_data noise;
    
    float * input, * output ;
    int sample_rate ;
    
    data_block_t db ;
} Distortion;

#endif
