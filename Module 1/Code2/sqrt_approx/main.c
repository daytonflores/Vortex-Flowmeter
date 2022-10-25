/*----------------------------------------------------------------------------
LAB EXERCISE 5.2 - SQUARE ROOT APPROXIMATION
 ----------------------------------------
Write an assembly code subroutine to approximate the square root of an 
argument using the bisection method. All math is done with integers, so the 
resulting square root will also be an integer

GOOD LUCK!
 *----------------------------------------------------------------------------*/

/*! \brief Calculates truncated (integer portion) square root of an integer in the range [0, (2^31) - 1]
 * 
 * This will be done using the Bisection Method (https://en.wikipedia.org/wiki/Bisection_method) with range [a, b] and midpoint c.
 * Initial values:
 *   a = 0 (smallest possible square root)
 *   b = 256 (largest possible square root)
 *   c = 128
 * The function will recursively calculate whether c is the exact root of input x.
 *   If so, then finish.
 *   Else if c^2 < x, then set lower bound a to c.
 *   Else if c^2 > x, then set upper bound b to c.
 * The midpoint c will be recalculated within the new range until either:
 *   c is the exact square root of x
 *   "new" c == "old" c (meaning integer approximation is within 1)
 * 
 * @param x The integer number within range of [0, (2^31) - 1] to find the truncated (integer portion) square root of
 */
__asm int my_sqrt(int x){
	;// set initial old c + new c midpoints in Bisection Method to 0
	MOV		r4, #0
	MOV		r5, #0

	;// if input is 0, then square root is 0 so branch to done
	CMP		r0, #0
	BEQ		done
	
	;// for dividing by 2 when finding midpoint c
	MOV		r1, #2

	;// set lower interval bound a to sqrt of min 32-bit value
	;// set upper interval bound b to sqrt of max 32-bit value
	MOV		r2, #0
	MOV		r3, #256

loop
	;// backup midpoint c to old c
	MOV		r5, r4

	;// calculate new c = (a + b)/2 
	ADDS	r4, r2, r3
	UDIV	r4, r4, r1

	;// if new c^2 equals input x, we found
	;// the exact integer square root so branch to done
	MUL		r6, r4, r4
	CMP		r6, r0
	BEQ		done
	
	;// else if new c^2 less than input x, set
	;// the lower bound a to new c and loop again
	BLT		setLowerBound
	
	;// else if new c^2 greater than input x, set
	;// the upper bound b to new c and loop again
	BGT		setUpperBound

setLowerBound
	MOV		r2, r4
	B			checkOldCAndNewC

setUpperBound
	MOV		r3, r4

	;// if old c == new c, we cannot get a closer
	;// approximation so branch to done
	;//
	;// else, try again with new bounds a and b
checkOldCAndNewC
	CMP		r4, r5
	BEQ		done
	B			loop

	;// found integer approximation of square root
	;// so store value in return register r0
	;// and branch with exchange to link register
done
	MOV		r0, r4
	BX		lr
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main(void){
	volatile int r, j=0;
	int i;
  r = my_sqrt(0);		// should be 0
  r = my_sqrt(25);		// should be 5
  r = my_sqrt(133);		// should be 11
  r = my_sqrt(2);		// should be 1 (0x00000001)
  r = my_sqrt(4);		// should be 2 (0x00000002)
  r = my_sqrt(22);		// should be 4 (0x00000004)
  r = my_sqrt(121);		// should be 11 (0x0000000B)
  for (i=0; i<10000; i++){
		r = my_sqrt(i);
    j+=r;
  }
	while(1)
		;
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
