# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include "defs.h"
/*
 * Author: Josh Johnson
 *
 * Unpack values from bit fields.
 *
 * Parameters:
 *   s - The bit field widths.
 *   p - The packed values.
 *
 * Returns - unpacked values.
 */

unpacked unpack(sizes s, packed p) {
	int sizeOfInt = sizeof(int) * 8;
	int i;
	int upper;
	int upperSize;
	int lower;
	int intNum = 0;
	int currentBits = sizeOfInt;
	int * unpackedInts = malloc(sizeof(int) * p.n);

	//fail if not enough memory
	if(unpackedInts == NULL){
		printf("Unable to allocate enough memory.");
		exit(0);
	}
	
	for(i = 0; i < s.numWidths; i++){
		//if entire integer is stored in this packed int
		if(currentBits - s.fieldWidths[i] >= 0){
			//shift left the amount of remaining bits, then shift back to get on the far right side
			unpackedInts[i] = ((unsigned int) (p.fieldValues[intNum] << (sizeOfInt - currentBits))) >> (sizeOfInt - s.fieldWidths[i]);
			currentBits -= s.fieldWidths[i];
		}else{
			//extract upper part of the packed int
			upper = ((unsigned int) (p.fieldValues[intNum] << (sizeOfInt - currentBits))) >> (sizeOfInt - s.fieldWidths[i]);

			intNum++; //move to next packed int
			//extract lower part of the packed int
			lower = ((unsigned int) p.fieldValues[intNum]) >> (sizeOfInt - (s.fieldWidths[i] - currentBits));

			//combine upper and lower ints
			unpackedInts[i] = (lower | upper);
			currentBits = sizeOfInt - s.fieldWidths[i] + currentBits;
		}
	}
	
	//create unpacked object
	unpacked un;
	un.values = unpackedInts;
	un.n = s.numWidths;
	return un;
} // end of unpack function 