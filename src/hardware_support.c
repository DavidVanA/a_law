#include <stdio.h>
#include <stdint.h>

int main(){
	FILE* sim_in;
	FILE* sim_out;
	int chunks;
	int16_t sample;
	uint8_t converted;
	uint32_t length;

	for( int i = 0; i < chunks/2; i++ ){
		fread( &sample, 2, 1, sim_in );
		
		__asm volatile( "ALAW %[converted],	%[sample]	\n"
			: [converted] "=r" (converted)
			: [sample] "r" (sample)
			: "cc"
		);

		fwrite( &converted, 1, 1, sim_out ); 

		length++;
	}

	return 0;
}
