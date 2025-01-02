# ifndef COMMON_H
# define COMMON_H 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef float DSP_SAMPLE;
//~ #define MAX_CHANNELS 2
#define TRUE true
#define FALSE false

#define MAX_SAMPLE (32767 << 8)
#define MIN_BUFFER_SIZE 128
#define MAX_BUFFER_SIZE 65536
#define MAX_CHANNELS 4
#define MAX_SAMPLE_RATE 48000

typedef struct {
    DSP_SAMPLE * __restrict__ data;
    DSP_SAMPLE * __restrict__ data_swap;
    int    len;
    int     channels;
    int sample_rate ;
} data_block_t;
static inline void *
gnuitar_memalign(size_t num, size_t bytes) {
    void *mem = NULL;
#ifndef __MINGW32__
    if (posix_memalign(&mem, 16, num * bytes)) {
        fprintf(stderr, "failed to allocate aligned memory.\n");
        exit(1);
    }
#else
    mem = __mingw_aligned_malloc(num * bytes, 16);
#endif
    assert(mem != NULL);

    memset(mem, 0, num * bytes);
    return mem;
}

# endif
