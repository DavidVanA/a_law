#ifndef _ALAW
#define _ALAW

#include "wav.h"

#undef TESTING

int a_law(WAV_Header *header, FILE *input, FILE *output);

#endif
