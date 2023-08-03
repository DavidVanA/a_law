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
The while loop used to find teh leading zeros chord has been replaced with a lookup table for log base 2:
``` C
const static int8_t log_table[128] =
{
	1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
     	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
     	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
     	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};
```
``` C
chord = log_table[(val >> 8) & 0x7F];
```

