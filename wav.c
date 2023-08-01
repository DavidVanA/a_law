/*
 * For processing of WAV files
 *
 * Written by David van Acken and Ryland Nezil
 *
 * On: June 11, 2023
 */

#include <stdio.h>
#include <stdint.h>

#include "wav.h"

bool get_wav_header(WAV_Header *header, FILE *input)
{
	fread(header, sizeof(WAV_Header), 1, input);
	return 0;
}

