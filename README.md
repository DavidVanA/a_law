# a_law

## Optimized code
Optimizations were made in 3 places. These optimizations increase runtime performance, and do not affect the quality of the compression.

### Finding sign
Finding the sign has been changed from an if/else check to two assembly instructions:
``` C
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
```

### Finding magnitude
The function to find the magnitude of a value has been changed from an if/else check to a string of assembly istructions:
``` C
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

```

### Calculating chord
The chord is calculated using the CLZ instruction
``` C
static int8_t get_leading_zeros(register uint32_t val) {
	register uint32_t num_zeros = 0;

	__asm volatile ( 
		"CLZ 	%[result],	%[input] \n"
	"	CMP 	%[result],	#24 \n"
	"	MOVGT 	%[result],	#24 \n"
    		: [result] "=r" (num_zeros)
    		: [input] "r" (val)
		: "cc"
  	);

	return 24 - num_zeros;
}
```

