#ifndef _ALAW
#define _ALAW

#include "wav.h"

#undef TESTING 

int a_law(WAV_Header *header, FILE *input, FILE *output);
int8_t a_law_convert(int16_t val);

#endif
