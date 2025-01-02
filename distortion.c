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
 *
 * $Log$
 * Revision 1.30  2006/08/06 20:14:54  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.29  2006/08/02 19:07:56  alankila
 * - add missing static declarations
 *
 * Revision 1.28  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.27  2005/09/04 19:30:23  alankila
 * - move the common clip code into a macro
 *
 * Revision 1.26  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.25  2005/09/04 12:12:35  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.24  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.23  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.22  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.21  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.20  2005/09/01 17:31:40  alankila
 * - various small fixes for multichannel / gui
 *
 * Revision 1.19  2005/08/27 18:11:35  alankila
 * - support 32-bit sampling
 * - use 24-bit precision in integer arithmetics
 * - fix effects that contain assumptions about absolute sample values
 *
 * Revision 1.18  2005/08/22 22:11:59  alankila
 * - change RC filters to accept data_block
 * - LC filters have no concept of "LOWPASS" or "HIGHPASS" filtering, there's
 *   just filter_no.
 * - remove unused SAMPLE8 typedef
 *
 * Revision 1.17  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.16  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.15  2005/08/12 11:24:05  alankila
 * - include math.h for isnan()
 *
 * Revision 1.14  2005/08/08 12:01:34  fonin
 * Include utils.h for isnan() on Windows
 *
 * Revision 1.13  2005/07/31 10:23:20  fonin
 * All processing in temp variable; added clipping code
 *
 * Revision 1.12  2005/04/29 11:24:43  fonin
 * Better presets
 *
 * Revision 1.11  2004/08/10 15:07:31  fonin
 * Support processing in float/int - type DSP_SAMPLE
 *
 * Revision 1.10  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.9  2003/03/11 22:04:00  fonin
 * Measure control sliders in standard units (ms, %).
 *
 * Revision 1.8  2003/03/09 20:49:45  fonin
 * Structures were redesigned to allow to change sampling params.
 *
 * Revision 1.7  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.6  2003/02/01 19:15:12  fonin
 * Use sizeof(variable) instead sizeof(type) in load/save procedures,
 * when reading/writing from file.
 *
 * Revision 1.5  2003/01/30 21:35:29  fonin
 * Got rid of rnd_window_pos().
 *
 * Revision 1.4  2003/01/29 19:34:00  fonin
 * Win32 port.
 *
 * Revision 1.3  2001/06/02 14:05:59  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.2  2001/03/25 12:10:14  fonin
 * Removed clip functionality. Effect control window ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:21:41  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "distortion.h"
#include "utils.h"

#include "log.h"

#define URI "http://shaji.in/plugins/fonin-distortion"

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Distortion * ap = calloc(1, sizeof(Distortion));
    ap -> sample_rate = sample_rate ;
    ap -> db.sample_rate = sample_rate ;
    ap -> db.channels = 1 ;
    ap -> noise.sample_rate = sample_rate ;
    ap -> fd.sample_rate = sample_rate ;
    
    return ap ;
}

static void
activate(LV2_Handle instance) {
    Distortion * ap = (Distortion *) instance ;
    
    * ap->sat = 33;
    * ap->level = 7.8;
    * ap->drive = 55.0;
    * ap->lowpass = 350;

    ap->noisegate = 3000;
    RC_setup(10, 1.5, &(ap->fd));

    RC_setup(10, 1, &(ap->noise));
    RC_set_freq(ap->noisegate, &(ap->noise));

    RC_set_freq(* (int *) ap->lowpass, &(ap->fd));
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	Distortion * params = (Distortion *) instance ;
	switch (port) {
		case INPUT:
			params -> input = (float *) data ;
			break ;
		case OUTPUT:
			params->output = (float *) data ;
			break ;
		case DRIVE:
			params->drive = (float *) data ;
			break ;
		case LEVEL:
			params->level = (float *) data ;
			break ;
		case SATURATION:
			params->sat = (float *) data ;
			break ;
		case LOWPASS_:
			params->lowpass = (float *) data ;
			break;
	}
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
    int             count,
                    currchannel = 0;
    DSP_SAMPLE     *s;
    Distortion * params = (Distortion *) instance ;
    struct distort_params *dp = (struct distort_params *) instance;
    double	    t;

    data_block_t * db = & params -> db ;
    params -> db.len = n_samples ;
    params -> db.channels = 1 ;
    
    for (int i = 0 ; i < n_samples ; i ++)
        params -> output [i] = params -> input [i] * (float)(1 << 23);
    
    params -> db.data = params -> output ;
    params -> db.data_swap = params -> output ;

    /*
     * sat clips derivative by limiting difference between samples. Use lastval 
     * member to store last sample for seamlessness between chunks. 
     */
    count = db->len;
    s = db->data;

    //~ RC_highpass(db, &(dp->fd));

    while (count) {
	/*
	 * apply drive  
	 */
	t=*s;
	t *= * dp->drive * 10;
	t /= 16;

	/*
	 * apply sat 
	 */
	if ((t - dp->lastval[currchannel]) > ((int) * dp->sat * 300 << 8))
	    t = dp->lastval[currchannel] + ((int) * dp->sat * 300 << 8);
	else if ((dp->lastval[currchannel] - t) > ((int) * dp->sat * 300 << 8))
	    t = dp->lastval[currchannel] - ((int) * dp->sat * 300 << 8);

	dp->lastval[currchannel] = t;
        currchannel = (currchannel + 1) % db->channels;
        
	t *= (int) * dp->level * 2.56;
	t /= 256;
#ifdef CLIP_EVERYWHERE
        CLIP_SAMPLE(t)
#endif
	if(isnan(t))
	    t=0;
	*s=t / (float)(1 << 23);

	s++;
	count--;
    }

    //~ LC_filter(db, 0, * dp->lowpass, &(dp->noise));
    
}

static void
deactivate(LV2_Handle instance) {

}

static void
cleanup(LV2_Handle instance) {
	free(instance);
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

static const LV2_Descriptor descriptor = {
	URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index) {
	switch (index) {
        case 0:  
            return &descriptor;
        default: 
            return NULL;
	}
}
