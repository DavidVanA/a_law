#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <valgrind/callgrind.h>

#include "alaw.h"

#define TEST_LEN 4
int16_t test_data [TEST_LEN] = { 0x0713, 0x0203, 0x0CDE, 0x0179 };

int main(int argc, char *argv[]) 
{
	printf("Starting test\n");

	if(argc >= 1)
	{
		if(0 == strcmp(argv[1], "-c"))
			printf("Compress");
		else if(0 == strcmp(argv[1], "-d"))
			printf("Decompress");
	}

	int8_t result[TEST_LEN];

	CALLGRIND_START_INSTRUMENTATION;
  	CALLGRIND_TOGGLE_COLLECT;

	// For every piece of test data
	for(int i = 0; i < TEST_LEN; i++)
		result[i] = a_law_convert(test_data[i]);

  	CALLGRIND_TOGGLE_COLLECT;
  	CALLGRIND_STOP_INSTRUMENTATION;
	printf("Test complete\n");
	for(int i = 0; i < TEST_LEN; i++)
	{
		printf("Input 	[%d]: %X\n", i, test_data[i]);
		printf("Result 	[%d]: %hhX\n", i, result[i]);
	}
}
