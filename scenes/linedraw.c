#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

#include "linedraw.h"  // not currently used

/*
* Draws a dot at (X0,Y0)
*/

void dot(unsigned int x0, unsigned int y0, unsigned char colour, UBYTE *chunky) /* coordinates at which to draw dot, with */
{
	chunky[y0*320+x0] = colour;
}

/*
* Draws a line in octant 0 or 3 ( |DeltaX| >= DeltaY ).
*/
void Octant0(unsigned int X0,
				 unsigned int Y0,
				 unsigned int DeltaX,
				 unsigned int DeltaY,
				 int XDirection,
				 unsigned char colour,
				 UBYTE *chunky)
// X0, Y0; /* coordinates of start of the line */
// DeltaX, DeltaY; /* length of the line (both > 0) */
// XDirection; /* 1 if line is drawn left to right, -1 if drawn right to left */
{
	int DeltaYx2;
	int DeltaYx2MinusDeltaXx2;
	int ErrorTerm;

	/* Set up initial error term and values used inside drawing loop */
	DeltaYx2 = DeltaY * 2;
	DeltaYx2MinusDeltaXx2 = DeltaYx2 - (int) ( DeltaX * 2 );
	ErrorTerm = DeltaYx2 - (int) DeltaX;

	/* Draw the line */
	dot(X0, Y0, colour, chunky); /* draw the first pixel */

	while ( DeltaX-- )
	{
		/* See if its time to advance the Y coordinate */
		if ( ErrorTerm >= 0 )
		{
			/* Advance the Y coordinate & adjust the error term	back down */
			Y0++;
			ErrorTerm += DeltaYx2MinusDeltaXx2;
		}
		else
		{
			/* Add to the error term */
			ErrorTerm += DeltaYx2;
		}

		X0 += XDirection; /* advance the X coordinate */
		dot(X0, Y0, colour, chunky); /* draw a pixel */
	}
}

/*
* Draws a line in octant 1 or 2 ( |DeltaX| < DeltaY ).
*/
void Octant1(unsigned int X0, unsigned int Y0, unsigned int DeltaX, unsigned int DeltaY, int XDirection, unsigned char colour, UBYTE *chunky)
// X0, Y0; /* coordinates of start of the line */
// DeltaX, DeltaY; /* length of the line (both > 0) */
// XDirection; /* 1 if line is drawn left to right, -1 if drawn right to left */
{
	int DeltaXx2;
	int DeltaXx2MinusDeltaYx2;
	int ErrorTerm;

	/* Set up initial error term and values used inside drawing loop */
	DeltaXx2 = DeltaX * 2;
	DeltaXx2MinusDeltaYx2 = DeltaXx2 - (int) ( DeltaY * 2 );
	ErrorTerm = DeltaXx2 - (int) DeltaY;

	dot(X0, Y0, colour, chunky); /* draw the first pixel */

	while ( DeltaY-- )
	{
		/* See if its time to advance the X coordinate */
		if ( ErrorTerm >= 0 )
		{
			/* Advance the X coordinate & adjust the error term	back down */
			X0 += XDirection;
			ErrorTerm += DeltaXx2MinusDeltaYx2;
		}
		else
		{
			/* Add to the error term */
			ErrorTerm += DeltaXx2;
		}

		Y0++; /* advance the Y coordinate */
		dot(X0, Y0, colour, chunky); /* draw a pixel */
	}
}
/*
* Draws a line on the EGA or VGA.
*/
void linedraw(int X0, int Y0, int X1, int Y1, UBYTE colour, UBYTE *chunky)
// X0, Y0; /* coordinates of one end of the line */
// X1, Y1; /* coordinates of the other end of the line */
// Color; /* color to draw line in */
{
	int DeltaX, DeltaY;
	int Temp;

	/* Save half the line-drawing cases by swapping Y0 with Y1
	and X0 with X1 if Y0 is greater than Y1. As a result, DeltaY
	is always > 0, and only the octant 0-3 cases need to be
	handled. */
	if ( Y0 > Y1 )
	{
		Temp = Y0;
		Y0 = Y1;
		Y1 = Temp;
		Temp = X0;
		X0 = X1;
		X1 = Temp;
	}

	/* Handle as four separate cases, for the four octants in which	Y1 is greater than Y0 */
	DeltaX = X1 - X0; /* calculate the length of the line in each coordinate */
	DeltaY = Y1 - Y0;
	if ( DeltaX > 0 )
	{
		if ( DeltaX > DeltaY )
		{
			Octant0(X0, Y0, DeltaX, DeltaY, 1, colour, chunky);
		}
		else
		{
			Octant1(X0, Y0, DeltaX, DeltaY, 1, colour, chunky);
		}
	}
	else
	{
		DeltaX = -DeltaX; /* absolute value of DeltaX */
		if ( DeltaX > DeltaY )
		{
			Octant0(X0, Y0, DeltaX, DeltaY, -1, colour, chunky);
		}
		else
		{
			Octant1(X0, Y0, DeltaX, DeltaY, -1, colour, chunky);
		}
	}
}
