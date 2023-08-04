
/*
 * A-Law compression algorithm
 *
 * Written by: David Van Acken and Ryland Nezil
 * On: June 11, 2023
 */

#include <stdio.h>
#include <valgrind/callgrind.h>

#include "alaw.h"


static inline uint8_t get_sign(register int16_t num) __attribute__((always_inline));
static int8_t get_leading_zeros(register uint32_t val);
static int16_t get_magnitude(int16_t val);

// 8-bit log table for fast log base 2
const static int8_t log_table[128] =
{
	1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
     	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
     	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
     	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

static uint16_t sign_mask = 0x8000;

int a_law(WAV_Header *header, FILE *input, FILE *output) {
	
	// Read chunk size (should match header plus size of header and 'data')
	uint32_t chunks;
	fread(&chunks, sizeof(chunks), 1, input);
#ifdef TESTING
	uint32_t header_chunks = header->size - sizeof(WAV_Header) + 4;
	if(chunks != header_chunks) {
		printf("Error: Header (%d) and data (%d) chunk sizes do not match\n", header_chunks, chunks);
		return 1;
	}
	printf("Number of chunks to read: %d\n", chunks);
	
	// Size of data to be read
	if(header->bits_per_sample != 16) {
		printf("Error: Data size is not 16 bits");
		return 1;
	}
#endif

	// Move past header in output file
	fseek(output, sizeof(WAV_Header) + sizeof(chunks), SEEK_SET);

	int16_t sample;		// Sample to be converted
	uint32_t length = 0;	// Total length of data
	uint8_t converted;	// Converted value to be written

	// For every chunk
	for(int i = 0; i < chunks/2; i++) {	
		// Read in one sample
		fread(&sample, 2, 1, input);
		// Shift over by number of bits needed to get next sample
		converted = a_law_convert(sample);
		// Write new data to output file
		fwrite(&converted, 1, 1, output);
		// Increment number of bytes written
		length++;
	}

	// Create new header for A-Law data
	WAV_Header output_header = {
		{ 'R', 'I', 'F', 'F' },		// "RIFF"
		length,				// Number of chunks
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

#ifdef TESTING
	// Print out the new header
	printf("\nOutput header:\n");
	print_header(&output_header);
#endif

	// Move back to beginning of file
	fseek(output, 0, SEEK_SET);
	// Write the new header
	fwrite(&output_header, sizeof(WAV_Header), 1, output);
	// Write the data length
	fwrite(&length, sizeof(chunks), 1, output);

	return 0;
}

int8_t a_law_convert(int16_t val) {
	
	int8_t converted, sign, chord, step;

	// Get sign bit (in correct position)
	sign = get_sign(val);
	// Get magnitude of value
	val = get_magnitude(val);

	// Get leading zero chord from value
	chord = log_table[(val >> 8) & 0x7F];
	// Get step data from correct position, ignore other data (+3 because 16-bit)
	step = (val >> (chord + 3) ) & 0x0F;

	// If the value is less than 256 clear the chord
	register uint32_t chord_long = chord;
	__asm volatile ( 
		"CMP 	%[val],		#256 \n"
	"	MOVLT 	%[chord],	#0 \n"
    	: [chord] "=r" (chord_long)
    	: [val] "r" (val)
	: "cc"
  	);
	
	// Get the converted value
	converted = sign | (chord <<  4) | step;
	
	// XOR converted value with 0x55 because PCM is weird
	return converted ^ 0x55;
}

static int16_t get_magnitude(register int16_t val) 
{
	register int16_t result;

	__asm volatile (
		"CMP 	%[val], 	#0 \n"
		"MOVGE 	%[result], 	%[val] \n"
		"MVNLT 	%[result], 	%[val] \n"
		"ADDLT 	%[result], 	%[result], 	#1 \n"
		: [result] "=r" (result)
		: [val] "r" (val)
		: "cc"
	);

	return result;
}

static inline uint8_t get_sign(register int16_t num) {
	register uint32_t result;

	__asm volatile (
		"ANDS %[result], %[num], %[sign_mask] \n" 
	    	"	MOVEQ %[result], #0x80 \n" 
		: [result] "=r" (result) 
		: [num] "r" (num), [sign_mask] "r" (sign_mask)
		: "cc"
	);

	return result;
}
