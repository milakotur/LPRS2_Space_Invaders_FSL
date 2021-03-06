/*
 * SI_draw.c
 *
 *  Created on: 16.06.2015.
 *      Author: Standzi, Marko, Dva geja (Urosko i Milan GAY)
 */

#include "SI_draw.h"
#include <math.h>

//koristimo za brisanje traga kad pomeramo donju ciglu
void erase_square(Xuint32 BaseAddress, Xuint8 x_crdnt, Xuint8 y_crdnt)
{
	int i, x, y, k;

	for (y = 0 + 32*y_crdnt; y < 32 + 32*y_crdnt; y++)
	{
		for(k = -7; k < 7; k++){
			x = x_crdnt + k;
			i = y*(640/8) + x;
			PUT_TO_FSL(i, 0x00000000);
		}
	}
}


void draw_ship(Xuint32 BaseAddress, Xuint8 x_crdnt, Xuint8 y_crdnt)
{
	int i, k, x, y;

	for (y = 2 + 32*y_crdnt; y < 14 + 32*y_crdnt; y++)
	{
		for(k = -7; k < 7; k++){
			x = x_crdnt + k;
			i = y*(640/8) + x;
			PUT_TO_FSL(i, 0x11111111);
		}
	}

}

void draw_brick(Xuint32 BaseAddress, Xuint8 x_crdnt, Xuint8 y_crdnt)
{
	int i, k, x, y;

	for (y = 2 + 32*y_crdnt; y < 14 + 32*y_crdnt; y++)
	{
		for(k = -2; k < 2; k++){
			x = x_crdnt + k;
			i = y*(640/8) + x;
			PUT_TO_FSL(i, 0x11111111);
		}
	}

}


void clear_graphics_screen(Xuint32 BaseAddress)
{
	int i;
	for (i = 0; i < 9600*4; i++){
	    PUT_TO_FSL(i, 0x00000000);
	}
}



void draw_ball(int x, int y, int x_prev, int y_prev)
{



	PUT_TO_FSL(((y_prev-6)*80 + x_prev),  0x00000000);
	PUT_TO_FSL(((y_prev-5)*80 + x_prev),  0x00000000);
	PUT_TO_FSL(((y_prev-4)*80 + x_prev),  0x00000000);
	PUT_TO_FSL(((y_prev-3)*80 + x_prev),  0x00000000);
	PUT_TO_FSL(((y_prev-2)*80 + x_prev),  0x00000000);
	PUT_TO_FSL(((y_prev-1)*80 + x_prev),  0x00000000);
	PUT_TO_FSL((y_prev*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+1)*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+2)*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+3)*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+4)*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+5)*80 + x_prev), 0x00000000);
	PUT_TO_FSL(((y_prev+6)*80 + x_prev), 0x00000000);

	PUT_TO_FSL(((y_prev-6)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev-5)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev-4)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev-3)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev-2)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev-1)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL((y_prev*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+1)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+2)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+3)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+4)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+5)*80 + x_prev+1), 0x00000000);
	PUT_TO_FSL(((y_prev+6)*80 + x_prev+1), 0x00000000);

	PUT_TO_FSL(((y-6)*80 + x), 0x00003333); //gore levo
	PUT_TO_FSL(((y-5)*80 + x), 0x00333333);
	PUT_TO_FSL(((y-4)*80 + x), 0x03333333);
	PUT_TO_FSL(((y-3)*80 + x), 0x33333333);
	PUT_TO_FSL(((y-2)*80 + x), 0x33333333);
	PUT_TO_FSL(((y-1)*80 + x), 0x33333333);
	PUT_TO_FSL((y*80 + x), 0x33333333);
	PUT_TO_FSL(((y+1)*80 + x), 0x33333333); //dole levo
	PUT_TO_FSL(((y+2)*80 + x), 0x33333333);
	PUT_TO_FSL(((y+3)*80 + x), 0x33333333);
	PUT_TO_FSL(((y+4)*80 + x), 0x03333333);
	PUT_TO_FSL(((y+5)*80 + x), 0x00333333);
	PUT_TO_FSL(((y+6)*80 + x), 0x00003333);

	PUT_TO_FSL(((y-6)*80 + x+1), 0x33330000); //gore desno
	PUT_TO_FSL(((y-5)*80 + x+1), 0x33333300);
	PUT_TO_FSL(((y-4)*80 + x+1), 0x33333330);
	PUT_TO_FSL(((y-3)*80 + x+1), 0x33333333);
	PUT_TO_FSL(((y-2)*80 + x+1), 0x33333333);
	PUT_TO_FSL(((y-1)*80 + x+1), 0x33333333);
	PUT_TO_FSL((y*80 + x+1), 0x33333333);
	PUT_TO_FSL(((y+1)*80 + x+1), 0x33333333); //dole desno
	PUT_TO_FSL(((y+2)*80 + x+1), 0x33333333);
	PUT_TO_FSL(((y+3)*80 + x+1), 0x33333333);
	PUT_TO_FSL(((y+4)*80 + x+1), 0x33333330);
	PUT_TO_FSL(((y+5)*80 + x+1), 0x33333300);
	PUT_TO_FSL(((y+6)*80 + x+1), 0x33330000);
}




