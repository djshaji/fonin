/*
 * GNUitar
 * Sustain effect
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
 * Revision 1.24  2006/08/06 20:14:55  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.23  2006/08/02 19:11:18  alankila
 * - add missing static declarations
 *
 * Revision 1.22  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.21  2005/09/04 19:30:23  alankila
 * - move the common clip code into a macro
 *
 * Revision 1.20  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.19  2005/09/04 12:12:36  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.18  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.17  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.16  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.15  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.14  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.13  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.12  2004/08/10 15:21:16  fonin
 * Support processing in float/int - type DSP_SAMPLE
 *
 * Revision 1.10  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.9  2003/03/12 20:54:04  fonin
 * Better presets.
 *
 * Revision 1.8  2003/03/11 22:04:00  fonin
 * Measure control sliders in standard units (ms, %).
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
 * Revision 1.2  2001/03/25 12:10:49  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:22:19  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include "sustain.h"
#include "log.h"

#define URI "http://shaji.in/plugins/fonin-sustain"

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Sustain * params = calloc(1, sizeof(Sustain));
    return params ;
}

static void
activate(LV2_Handle instance) {
    Sustain * psustain = (Sustain *) instance ;
    * psustain->noise = 40 / 2.56;
    * psustain->sust = 256 / 2.56;
    * psustain->threshold = 256 / 2.56;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    Sustain * params = (Sustain *) instance ;
    switch (port) {
        case INPUT:
            params -> input = (float *) data ;
            break ;
        case OUTPUT:
            params -> output = (float *) data ;
            break ;
        case GATE:
            params -> threshold = (float *) data ;
            break ;
        case SUSTAIN:
            params -> sust = (float *) data ;
            break ;
        case NOISE:
            params -> noise = (float *) data ;
            break ;
    }
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{    
    int             count;
    DSP_SAMPLE     *s;
    struct sustain_params *ds;
    DSP_SAMPLE      volAccum,
                    tmp;
    float           CompW1;
    float           CompW2;
    float           gateFac;
    float           compFac;

    ds = (struct sustain_params *) instance;
    data_block_t * db = & ds -> db ;
    ds -> db.len = n_samples ;
    ds -> db.channels = 1 ;
    
    for (int i = 0 ; i < n_samples ; i ++)
        ds -> output [i] = ds -> input [i] * (float)(1 << 23);
    
    ds -> db.data = ds -> output ;
    ds -> db.data_swap = ds -> output ;

    count = db->len;
    s = db->data;

    volAccum = ds->volaccum;
    CompW1 = * ds->sust * 2.56/ 100.0f;
    CompW2 = 1.0f - CompW1;

    while (count) {
	tmp = *s;
	/*
	 * update volAccum 
	 */
	tmp = (tmp < 0) ? -tmp : tmp;
	volAccum = (256 - * ds->noise * 2.56) * volAccum + * ds->noise * 2.56 * tmp;
	volAccum /= 256;

	/*
	 * handle compression 
	 */
	compFac = 30000.0f / (float) volAccum;
	compFac = CompW1 * compFac + CompW2;
	/*
	 * handle gate 
	 */
	if (* ds->threshold * 2.56 <= 1.0f)
	    gateFac = 1.0f;
	else
	    gateFac = (volAccum > (* ds->threshold * 2.56 * 100)) ? 1.0f :
		((float) (volAccum) / (float) (* ds->threshold * 2.56 * 100));
	/*
	 * process signal... 
	 */
	tmp = ((float) (*s) * compFac * gateFac);
#ifdef CLIP_EVERYWHERE
        CLIP_SAMPLE(tmp)
#endif
	*s = tmp / (float)(1 << 23);
	s++;
	count--;
    }
    ds->volaccum = volAccum;
    
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
