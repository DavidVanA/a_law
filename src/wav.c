/*
 * For processing of WAV files
 *
 * Written by: David Van Acken
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

bool print_header(WAV_Header *header)
{
	printf("RIFF: %c%c%c%c\n", 
			header->riff_label[0],
			header->riff_label[1],
			header->riff_label[2],
			header->riff_label[3]);
	printf("Size: %d\n", header->size);
	printf("WAVE: %c%c%c%c\n", 
			header->wave_label[0],
			header->wave_label[1],
			header->wave_label[2],
			header->wave_label[3]);
	printf("fmt: %c%c%c%c\n", 
			header->fmt_label[0],
			header->fmt_label[1],
			header->fmt_label[2],
			header->fmt_label[3]);
	printf("Data len: %d\n", header->data_length);
	printf("Type: %d\n", header->type);
	printf("Num channels: %d\n", header->num_channels);
	printf("Sample Rate: %d\n", header->sample_rate);
	printf("Byte Rate: %d\n", header->byte_rate);
	printf("Container size: %d\n", header->container_size);
	printf("Bits per sample: %d\n", header->bits_per_sample);
	printf("Data: %c%c%c%c\n", 
			header->data_label[0],
			header->data_label[1],
			header->data_label[2],
			header->data_label[3]);
}
