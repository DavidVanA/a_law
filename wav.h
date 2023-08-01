#ifndef _WAV
#define _WAV

#include <stdbool.h>
#include <stdint.h>

typedef struct WAV_Header
{
	char riff_label[4];
	uint32_t size;
	char wave_label[4];
	char fmt_label[4];
	uint32_t data_length; // # bytes in format section
	uint16_t type;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t container_size; // block align in bytes
	uint16_t bits_per_sample;
	char data_label[4];
} WAV_Header;

bool get_wav_header(WAV_Header *header, FILE *input);

#endif
