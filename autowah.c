/*
 * GNUitar
 * Autowah effect
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
 * Revision 1.48  2009/03/09 13:27:51  alankila
 * Add TB-303 style filter as an alternative.
 *
 * Revision 1.47  2006/12/01 13:21:25  alankila
 * - reduce autowah gain to avoid clipping so harshly
 *
 * Revision 1.46  2006/10/27 21:54:46  alankila
 * - new source file: audio-midi.c. Do some data abstraction, prepare to
 *   support multiple midi continuous controls.
 *
 * Revision 1.45  2006/10/27 18:16:39  alankila
 * - new GUI that will replace all effect GUIs in GNUitar with time. It needs
 *   some abstracting now.
 *
 * Revision 1.44  2006/08/08 22:36:33  alankila
 * - use aligned memory for autowah due to biquads
 *
 * Revision 1.43  2006/08/08 21:05:31  alankila
 * - optimize gnuitar: this breaks dsound, I'll fix it later
 *
 * Revision 1.42  2006/08/07 20:15:41  alankila
 * - remove dependency on rcfilter: use biquad band-pass instead of the
 *   repeated RC pass filter.
 *
 * Revision 1.41  2006/08/06 20:14:54  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.40  2006/07/28 20:38:55  alankila
 * - midi control into autowah, parameter lickup
 *
 * Revision 1.39  2006/07/16 12:40:04  alankila
 * - dirty the moog sound a bit to better justify its existence
 *
 * Revision 1.38  2006/07/16 12:26:38  alankila
 * - add more wah algorithms, including gnuitar's original
 *
 * Revision 1.37  2006/05/31 14:05:10  fonin
 * Added #include for sanity
 *
 * Revision 1.36  2006/05/20 17:30:29  alankila
 * - use parameters that better demonstrate the "wahness" of the effect
 *
 * Revision 1.35  2006/05/07 10:25:17  alankila
 * - I misspelled Antti's name.
 *
 * Revision 1.34  2006/05/07 07:40:14  alankila
 * - still more parameter finetuning
 *
 * Revision 1.33  2006/05/07 07:11:46  alankila
 * - fix load/save function
 * - add static keywords where appropriate
 * - allow user to tune the resonance. Resonance values near 110 % trigger
 *   self-supporting oscillations of the moog filter, and are actually used
 *   by real moog synths for sound generation. I capped it at 120 % so that
 *   user can play with that a bit.
 *
 * Revision 1.32  2006/05/06 16:52:29  alankila
 * - better defaults
 * - more exciting Wah algorithm according to Antti Huovilainen's paper
 *   "Non-Linear Digital implementation Of The Moog Ladder Filter"
 *   (published on 7th int. conference on digital audio effects, DAFx'04)
 *
 * Revision 1.31  2005/10/22 14:21:05  alankila
 * - reduce wah sweep range to saner limits, also reduce number of magical-
 *   seeming constants. This version is most reliable pick detector yet.
 *
 * Revision 1.30  2005/10/22 13:55:14  alankila
 * - add history buffer to make wah behaviour sampling parameter independent.
 * - add rudimentary handling for multiple channels by ignoring all but first
 *   in the pick-sweep mode.
 *
 * Revision 1.29  2005/10/22 13:04:43  alankila
 * - add power and delta computation so that wah may key on either type of
 *   increase. Also convert units to dB.
 *
 * Revision 1.28  2005/10/21 19:19:48  alankila
 * - add a pick-sensitive mode where a downward sweep is started synchronously
 *   with picking
 * - reduce wah strength and gain to help with clipping and make effect more
 *   usable (more a wowwow-type sound and less resonance)
 *
 * Revision 1.27  2005/10/02 08:25:25  fonin
 * "Mix" checkbox converted to dry/wet slider
 *
 * Revision 1.26  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.25  2005/09/04 19:45:12  alankila
 * - replace "Speed 1/ms" with "Period ms" which is easier to understand
 *
 * Revision 1.24  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.23  2005/09/04 12:12:35  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.22  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.21  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.20  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.19  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.18  2005/09/01 16:22:42  alankila
 * - oops, revert gain change
 *
 * Revision 1.17  2005/09/01 16:20:21  alankila
 * - fix comment, and reduce autowah gain
 *
 * Revision 1.16  2005/09/01 16:09:54  alankila
 * - make rcfilter and autowah multichannel ready. In addition, autowah
 *   now performs linear sweep in logarithmic domain rather than exponential.
 *
 * Revision 1.15  2005/08/22 22:11:59  alankila
 * - change RC filters to accept data_block
 * - LC filters have no concept of "LOWPASS" or "HIGHPASS" filtering, there's
 *   just filter_no.
 * - remove unused SAMPLE8 typedef
 *
 * Revision 1.14  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.13  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.12  2005/04/29 11:24:59  fonin
 * Return back the 1.5 amplify coeff
 *
 * Revision 1.11  2005/04/26 13:37:39  fonin
 * Declaring dry[] in the autowah_filter as static fixes the 100% CPU usage on windows; also amplify coefficients changed from 1.5 to 1.0
 *
 * Revision 1.10  2004/08/10 15:07:31  fonin
 * Support processing in float/int - type DSP_SAMPLE
 *
 * Revision 1.9  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.8  2003/03/09 20:46:07  fonin
 * - parameter "speed" removed from effect internal structure, df (delta ef)
 *   instead;
 * - cleanup of dead code and variables in autowah_filter();
 * - parameter speed is measured in 1 wave per N msec.
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
 * Revision 1.3  2001/06/02 14:05:58  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.2  2001/03/25 12:10:49  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:21:05  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include "autowah.h"
#include "audio-midi.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define tanhf(x) tanh(x)
#define expf(x) exp(x)
#endif

/* these thresholds are used to trigger the sweep. The system accumulates
 * time-weighted average of square difference between samples ("delta") and
 * the energy per sample ("power"). If either suddenly increases, the
 * sweep triggers. Data is collected AUTOWAH_HISTORY_LENGTH ms apart. */

#define AUTOWAH_HISTORY_LENGTH  100  /* ms */
#define AUTOWAH_DISCANT_TRIGGER 0.60 /* dB */
#define AUTOWAH_BASS_TRIGGER    0.65 /* dB */

static const char *methods[] = {
    "Lowpass",
    "Bandpass",
    "Moog ladder",
    "TB-303 style",
    NULL
};

static const char *syncs[] = {
    "Envelope",
    "Continuous",
    "MIDI continous control",
    NULL
};

/* sin table */
#define SIN_LOOKUP_SIZE         65536
float sin_lookup_table[SIN_LOOKUP_SIZE+1];

static void init_sin_lookup_table(void);
static inline float
sin_lookup(float pos) {
    return sin_lookup_table[(int) (pos * (float) SIN_LOOKUP_SIZE)];
}

static void
init_sin_lookup_table(void) {
    int i = 0;
    for (i = 0; i < SIN_LOOKUP_SIZE + 1; i += 1)
        sin_lookup_table[i] = sin(2 * M_PI * i / SIN_LOOKUP_SIZE);
}

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    AutoWah * ap = gnuitar_memalign(1, sizeof(struct autowah_params));
    ap -> sample_rate = sample_rate ;
    ap->history = new_Backbuf(MAX_SAMPLE_RATE * AUTOWAH_HISTORY_LENGTH / 1000);

    init_sin_lookup_table ();
    return ap;    
}

static void
activate(LV2_Handle instance) {
    AutoWah * ap = (AutoWah *) instance ;
    * ap->method = 0; /* low-pass resonant filter */
    * ap->freq_low = 280;
    * ap->freq_high = 900;
    * ap->sweep_time = 500;
    * ap->drywet = 100;
    * ap->sync = 0;
    * ap->res = 85;
}

static double
power2db(double power)
{
    return log(power) / log(10) * 10;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    AutoWah * params = (AutoWah *) instance ;
    switch (port) {
        case INPUT:
            params -> input = (float *) data ;
            break ;
        case OUTPUT:
            params -> output = (float *) data ;
            break ;
        case CONTROL:
            params -> sync = (float *) data ;
            break ;
        case PERIOD:
            params -> sweep_time = (float *) data ;
            break ;
        case LAST_FREQ:
            params -> freq_low = (float *) data ;
            break ;
        case FIRST_FREQ:
            params -> freq_high = (float *) data ;
            break ;
        case RESONANCE:
            params -> res = (float *) data ;
            break ;
        case DRY_WET:
            params -> drywet = (float *) data ;
            break ;
        case WAH_TYPE:
            params -> method = (float *) data ;
            break ;
    }
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
    struct autowah_params *ap;
    int             i, curr_channel = 0, delay_time;
    float           freq, g, g2;

    ap = (struct autowah_params *) instance;
    AutoWah * ds = (struct autowah_params *) instance;
    data_block_t * db = & ds -> db ;
    ds -> db.len = n_samples ;
    ds -> db.channels = 1 ;
    
    for (int i = 0 ; i < n_samples ; i ++)
        ds -> output [i] = ds -> input [i] * (float)(1 << 23);
    
    ds -> db.data = ds -> output ;
    ds -> db.data_swap = ds -> output ;

    //~ memcpy(db->data_swap, db->data, db->len * sizeof(DSP_SAMPLE));

    if (*ap->sync == 1) { /* continuous sweep */
        /* recover from noncontinuous sweep */
        if (ap->dir == 0)
            ap->dir = 1.0;
        
        if (ap->f > 1.0f && ap->dir > 0)
            ap->dir = -1;
        if (ap->f < 0.0f && ap->dir < 0)
            ap->dir = 1;
    }
    if (ap->sync == 0) { /* envelope detect */
        /* Firstly, quiesce wah if we have reached the end of sweep */
        if (ap->f < 0.0f) {
            ap->f = 0.0;
            ap->dir = 0;
        }
        delay_time = ap -> sample_rate * AUTOWAH_HISTORY_LENGTH / 1000;
        
        /* Estimate signal higher frequency content's power. When user picks
         * the string strongly it's the high frequency content that increases
         * most. */

        /* XXX we should probably treat all channels separately.
         * We just skip all channels but the first presently. */
        for (i = 0; i < db->len; i++) { if (curr_channel == 0) {
            ap->delayed_accum_power += pow(ap->history->get(ap->history, delay_time), 2);
            ap->fresh_accum_power += pow(db->data[i], 2);
            
            ap->delayed_accum_delta +=
                pow(ap->history->get(ap->history, delay_time) -
                    ap->history->get(ap->history, delay_time - 1), 2);
            
            ap->fresh_accum_delta +=
                    pow(db->data[i] - ap->history->get(ap->history, 0), 2);

            ap->history->add(ap->history, db->data[i]);
            
            ap->accum_n += 1;
            if (ap->accum_n > 8192) {
                ap->fresh_accum_power   /= 2; 
                ap->fresh_accum_delta   /= 2; 
                ap->delayed_accum_power /= 2; 
                ap->delayed_accum_delta /= 2;
                ap->accum_n             /= 2;
            }
        } curr_channel = (curr_channel + 1) % db->channels; }

        /* I skip some scale factors here that would cancel out */
        if ((power2db(ap->fresh_accum_delta) > power2db(ap->delayed_accum_delta)
                                              + AUTOWAH_DISCANT_TRIGGER) ||
            (power2db(ap->fresh_accum_power) > power2db(ap->delayed_accum_power)
                                              + AUTOWAH_BASS_TRIGGER)) {
            ap->f = 1.0f;
            ap->dir = -1.0;
        }
    }
    if (*ap->sync == 2) { /* midi control, from Roland FC-200 or somesuch */
        ap->f = midi_get_continuous_control(-1);
        ap->dir = 0;
        ap->freq_vibrato = 0;
    }

    /* in order to have audibly linear sweep, we must map
     * [0..1] -> [freq_low, freq_high] linearly in log2, which requires
     * f(x) = a * 2 ^ (b * x)
     *
     * we know that f(0) = freq_low, and f(1) = freq_high. It follows that:
     * a = freq_low, and b = log2(freq_high / freq_low)
     */

    ap->smoothed_f = (ap->f + ap->smoothed_f) / 2.f;
    freq = *ap->freq_low * pow(2, log(*ap->freq_high / *ap->freq_low)/log(2) * ap->smoothed_f + 0.2 * sin_lookup(ap->freq_vibrato));
    ap->f += ap->dir * 1000.0f / *ap->sweep_time * db->len / (ap -> sample_rate * db->channels) * 2;

    ap->freq_vibrato += 1000.0f / *ap->sweep_time * db->len / (ap -> sample_rate * db->channels) / 2.0f;
    if (ap->freq_vibrato >= 1.0f)
        ap->freq_vibrato -= 1.0f;
    
    switch ((int)*ap->method) {
      case 0:
        /* lowpass resonant filter -- we must avoid setting value 0 to
         * resonance. We also drop level by 6 dB to leave some room for it. */
        set_lpf_biquad(ap -> sample_rate, freq, 1.1 + -*ap->res / 100.0, &ap->lpf);
        for (i = 0; i < db->len; i += 1) {
            db->data[i] = do_biquad(db->data[i], &ap->lpf, curr_channel) / 2;
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;
        
      case 1: 
        set_bpf_biquad(ap->sample_rate, freq, 1.1 + -*ap->res / 100.0, &ap->bpf);
        for (i = 0; i < db->len; i += 1) {
            db->data[i] = do_biquad(db->data[i], &ap->bpf, curr_channel);
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;

      case 2:
      case 3:
        /* Moog ladder filter according to Antti Huovilainen. */

/* I, C, V = electrical parameters
 * f = center frequency
 * r = resonance amount 0 .. 1
 *
 * ya(n) = ya(n-1) + I / (C * f) * (tanh( (x(n) - 4 * r * yd(n-1)) / (2 * V) ) - Wa(n-1))
 * yb(n) = yb(n-1) + I / (C * f) * (Wa(n) - Wb(n-1))
 * yc(n) = yc(n-1) + I / (C * f) * (Wb(n) - Wc(n-1))
 * yd(n) = yd(n-1) + I / (C * f) * (Wc(n) - Wd(n-1))
 *
 * Wx = tanh(Yx(n) / (2 * Vt)) */

        g = 1.f - expf((float) (-2.0 * M_PI) * freq / ap->sample_rate);
        g2 = g;
        /* TB-303 style: the first phase is one octave higher than rest */
        if (*ap->method == 3)
            g2 = 1.f - expf((float) (-2.0 * M_PI) * 2 * freq / ap->sample_rate);
        for (i = 0; i < db->len; i += 1) {
#define PARAM_V (MAX_SAMPLE * 1.0) /* the sound gets dirtier if the factor gets small */
            ap->ya[curr_channel] += (float) PARAM_V * g2 *
                (tanhf( (db->data[i] - 4.f * *ap->res/100.0f * ap->yd[curr_channel]) / (float) PARAM_V )
                 - tanhf( ap->ya[curr_channel] / (float) PARAM_V));
            ap->yb[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->ya[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yb[curr_channel] / (float) PARAM_V ));
            ap->yc[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->yb[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yc[curr_channel] / (float) PARAM_V ));
            ap->yd[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->yc[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yd[curr_channel] / (float) PARAM_V ));

            /* the wah causes a gain loss of 12 dB which, but due to
             * resonance we may clip; to compromise I'll adjust 6 dB back. */
            db->data[i] = ap->yd[curr_channel] * 2.f;
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;

      default:
        break;
    }
    
    /* mix with dry sound */
    for (i = 0; i < db->len; i++) {
        db->data[i] = (db->data[i]**ap->drywet + db->data_swap[i]*(100.f-*ap->drywet))/100.0f;    
        db -> data [i] = db -> data [i] / (float)(1 << 23) ;
    }
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
