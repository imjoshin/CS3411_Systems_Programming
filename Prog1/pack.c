# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include "defs.h"
/*
 * Author: Josh Johnson
 *
 * Pack values into bit fields.
 *
 * Parameters:
 *   s - The bit field widths.
 *   un - The unpacked values.
 *
 * Returns - packed values.
 */

packed pack(sizes s, unpacked un) {
	int sizeOfInt = sizeof(int) * 8;
	int i;
	int temp;
	int intNum = 0;
	int currentBits = 0;
	
	//count number of bits total
	for(i = 0; i < s.numWidths; i++)
		currentBits += s.fieldWidths[i];

	//allocate data for array of required size
	int * packedInts = malloc(sizeof(int) * (currentBits / sizeOfInt + 1));
	if(packedInts == NULL){
		printf("Unable to allocate memory.");
		exit(0);
	}

	currentBits = 0;
	for(i = 0; i < s.numWidths; i++){
		//if next integer and fit into the current packed int
		if(currentBits + s.fieldWidths[i] <= sizeOfInt){
			//move all contents up to make room for next integer
			packedInts[intNum] = packedInts[intNum] << s.fieldWidths[i];
			packedInts[intNum] = packedInts[intNum] | un.values[i];
			currentBits += s.fieldWidths[i];
		}else{
			//split integer up. first grab the top portion that will fit in the last bits of current int
			temp = ((unsigned int) un.values[i]) >> (s.fieldWidths[i] - (sizeOfInt - currentBits));
			//shift packedInt to make room for top portion
			packedInts[intNum] = packedInts[intNum] << (sizeOfInt - currentBits);
			packedInts[intNum] = packedInts[intNum] | temp;

			intNum++; //move to next packed int
			currentBits = s.fieldWidths[i] - (sizeOfInt - currentBits);
			//grab lower portion and put it in the next packed int
			temp = ((unsigned int) (un.values[i] << ((sizeOfInt - currentBits)))) >> (sizeOfInt - currentBits);
			packedInts[intNum] = temp;
		}
	}

	//shift last packed int to the top
	packedInts[intNum] <<= (sizeOfInt - currentBits);

	//create packed object
	packed p;
	p.fieldValues = packedInts;
	p.n = intNum + 1;
	return p;
} // end of pack function 