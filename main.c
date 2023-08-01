'/*
 * A-Law compression algorithm
 *
 * Written by: David Van Acken and Ryland Nezil
 * On: June 11, 2023
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "wav.h"
#include "alaw.h"

int main(int argc, char *argv[]) {
	// Check argements
	if(argc < 3) {
		// Print error message
		printf("Usage: alaw.x <input> <output>\n"); 
		return 1;
	}

	// Try to open the input file
	FILE *input = fopen(argv[1], "rb");
	FILE *output = fopen(argv[2], "wb");
	// Check if the input was opened
	if(input == NULL)
	{
		printf("Error: Could not open input file\n");
		return 1;
	}
	printf("File opened: %s\n", argv[1]);
	if(output == NULL) {
		printf("Error: Could not open output file\n");
		return 1;
	}
	printf("File opened: %s\n", argv[2]);

	// Extract and print header from file
	WAV_Header input_header;
	get_wav_header(&input_header, input);

	// Compress and output
	a_law(&input_header, input, output);

	fclose(input);
	fclose(output);

	return 0;
}
