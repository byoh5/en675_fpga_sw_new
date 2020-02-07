//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		enx_math.c
//	Description:
//	Author:
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	180614		hjlee		Add floorSqrt
// -----------------------------------------------------------------------------
#include "dev.h"

// Returns floor of square root of x
UINT floorSqrt(UINT x)
{
	// Base cases
	if (x == 0 || x == 1) {
		return x;
	}

	// Do Binary Search for floor(sqrt(x))
	UINT start = 1, end = x / 2, ans = 0;
	while (start <= end) {
		UINT mid = (start + end) / 2;

		// If x is a perfect square
		if (mid * mid == x) {
			return mid;
		}

		// Since we need floor, we update answer when mid*mid is 
		// smaller than x, and move closer to sqrt(x)
		if (mid * mid < x) {
			start = mid + 1;
			ans = mid;
		} else { // If mid*mid is greater than x
			end = mid - 1;
		}
	}
	return ans;
}