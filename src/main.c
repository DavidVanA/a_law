/*
 * A-Law compression algorithm
 *
 * Written by: David Van Acken
 * On: June 11, 2023
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "wav.h"
#include "alaw.h"

int main(int argc, char *argv[]) {
	// Check argements
	if(argc < 4) {
		// Print error message
		printf("Usage: alaw.x -<c/d> <input> <output>\n"); 
		return 1;
	}

	// Try to open the input file
	FILE *input = fopen(argv[2], "rb");
	FILE *output = fopen(argv[3], "wb");
	// Check if the input was opened
	if(input == NULL)
	{
		printf("Error: Could not open input file\n");
		return 1;
	}
	if(output == NULL) {
		printf("Error: Could not open output file\n");
		return 1;
	}
#ifdef TESTING
	printf("File opened: %s\n", argv[1]);
	printf("File opened: %s\n", argv[2]);
#endif

	// Extract and print header from file
	WAV_Header input_header;
	get_wav_header(&input_header, input);
#ifdef TESTING
	print_header(&input_header);
#endif

	// TODO: If header looks good
	// Compress and output
	if(0 == strcmp(argv[1], "-c"))
	{
		a_law(&input_header, input, output);
	}

	else if(0 == strcmp(argv[1], "-d"))
	{
		a_law_decomp(&input_header, input, output);
	}

	fclose(input);
	fclose(output);

	return 0;
}
