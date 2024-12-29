# ifndef COMMON_H
# define COMMON_H 1

typedef float DSP_SAMPLE;
#define MAX_CHANNELS 1
#define TRUE true
#define FALSE false

#define MAX_SAMPLE (32767 << 8)

typedef struct {
    DSP_SAMPLE * __restrict__ data;
    DSP_SAMPLE * __restrict__ data_swap;
    int    len;
    int     channels;
} data_block_t;

# endif
