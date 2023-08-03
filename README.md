# a_law

## Unoptimized code
There are 3 places in this code where optimization has been intentionally avoided. They are:

### Finding sign
The sign is found with an if/else
``` C
static uint8_t get_sign(int16_t num) {

	if(num < 0)
		return 0x00;
	else
		return 0x80;
}
```

### Finding magnitude
The magnitude is found with an if/else
``` C
static int16_t get_magnitude(int16_t val) 
{
	if(val >= 0)
		return val;
	else
		// Return positive value (+1 to avoid -MAX issues)
		return -(val+1);	
}
```

### Calculating chord
Calculating the chord is done with a fairly complicated while loop
``` C
static uint8_t get_leading_zero_chord(int16_t val) {
	// Get chord according to upper bits
	uint8_t chord = 7;
	while( (!(val & 0x4000)) && chord != 0 )
	{
		val = val << 1;
		chord --;
	}

	return chord;
}
```
