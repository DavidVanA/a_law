
/*
 * A-Law compression algorithm
 *
 * Written by: David Van Acken and Ryland Nezil
 * On: June 11, 2023
 */

#include <stdio.h>

#include "alaw.h"

#define ALAW_A (87.6)

static uint8_t a_law_convert(uint32_t val);
static uint8_t get_sign(uint32_t num);
static int get_leading_zeros(uint32_t val);
static uint8_t get_leading_zero_chord(int num_zeros);
static uint8_t get_compressed_data(uint32_t val, int num_zeros);

int a_law(WAV_Header *header, FILE *input, FILE *output) {
	// Get data container size
	int data_size = header->container_size/header->num_channels;
	
	// Make sure that this data can be compressed
	if(header->bits_per_sample < 12 || header->bits_per_sample > 32) {
		printf("Error: Invalid number of bits per sample\n");
		return 1;
	}

	// Read chunk size (should match header plus size of header and 'data')
	uint32_t chunks;
	uint32_t header_chunks = header->size - sizeof(WAV_Header) + 4;
	fread(&chunks, sizeof(chunks), 1, input);

	// Move past header in output file
	fseek(output, sizeof(WAV_Header), SEEK_SET);

	// For every chunk
	uint32_t sample;
	uint32_t size = 0;
	uint8_t converted;
	
	// Want 12 bits per sample, so make data into that format
	int shift_amount = header->bits_per_sample - 12;

	// Unsure about this line
	int read_size = header->container_size/header->num_channels;

	for(int i = 0; i < chunks/2; i++) {
		// Read in one sample
		fread(&sample, read_size, 1, input);

		// For every sample stored in this 32-bit int
		// Shift over by number of bits needed to get next sample
		converted = a_law_convert(sample >> shift_amount);
		fwrite(&converted, 1, 1, output);
		size++;
	}

	WAV_Header output_header = {
		{ 'R', 'I', 'F', 'F' },		// "RIFF"
		size,				// Number of chunks
		{ 'W', 'A', 'V', 'E' },		// "WAVE"
		{ 'f', 'm', 't', ' ' },	// "fmt "
		0x00000010,			// # format bytes
		0x0006,				// A-Law format
		header->num_channels,		// Number of channels
		header->sample_rate,		// Sample rate
		header->sample_rate * header->num_channels, // Byte rate
		header->num_channels,		// Container size
		8,				// Bits per sample
		{ 'd', 'a', 't', 'a' },		// "data"
	};

	fseek(output, 0, SEEK_SET);
	fwrite(&output_header, sizeof(WAV_Header), 1, output);

	return 0;
}

static uint8_t a_law_convert(uint32_t val) {
	// Get value without sign bit
	uint32_t val_signless = val & 0x7FF;
	
	// Number of zeros is <leading zeros> - <space before data> - <space for sign>
	int num_zeros = get_leading_zeros(val_signless) - 20;

	// Get the converted value
	uint8_t converted = get_leading_zero_chord(num_zeros) | 
			    get_compressed_data(val_signless, num_zeros) |
			    get_sign(val);

	// Return the converted value
	return converted;
}

static int get_leading_zeros(uint32_t val) {
	int num_zeros;

	__asm ("CLZ %[result], %[input]"
    		: [result] "=r" (num_zeros)
    		: [input] "r" (val)
  	);

	// Use predicate operation here
	if( num_zeros > 26 ){
		num_zeros = 27;
	}

	return num_zeros;
}

static uint8_t get_compressed_data(uint32_t val, int num_zeros)
{
	uint32_t masked_data;
	uint8_t step_data;

	// Use predicate operations 	
	if( num_zeros < 7 ){
		// Find position of relevant data
		int data_position = 7 - num_zeros;
		
		// Get relevant data
		masked_data = val & (0x0F << data_position);
		
		// Shift data down to start and put into 8-bit int
		step_data = (uint8_t)(masked_data >> data_position);
	}else{
		// Asymmetric case
		masked_data = val & (0xf << 1 );
		step_data = (uint8_t)(masked_data >> 1);
	}

	return step_data;
}

static uint8_t get_leading_zero_chord(int num_zeros) {
	// Chord is 0x70 for no leading zeros, 0 for max number
	return (0x07 - num_zeros) << 4;
}

static uint8_t get_sign(uint32_t num) {
	uint32_t mask = 0x1000;
	uint32_t result;

	asm ("ANDS %[result], %[num], %[mask]" 
		: [result] "=r" (result) 
		: [num] "r" (num), [mask] "r" (mask));

	asm ("MOVNE %[result], #0x80" 
		: [result] "=r" (result) 
		:
	);

	return result;
}
