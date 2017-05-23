/*
 * SI_logic.c
 *
 *  Created on: 16.06.2015.
 *      Author: student
 */

#include "SI_logic.h"

Xint8 ship_hit_projectile(int i)
{
	if(projectiles_map[i][SHIP_Y] == 2)
	{
		projectiles_map[i][SHIP_Y] = 0;
		projectiles_of_invaders_num[i]--;
		return 1;
	}
	else
		return 0;
}

void move_spaceship(Xuint8 *dir)
{
	erase_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, spaceship_x, SHIP_Y);
	if(*dir == 0)
	{
		if(spaceship_x == MAX_SHIP_X)
		{
			spaceship_x--;
			*dir = 1;
		}
		else
			spaceship_x++;
	}
	else if(spaceship_x == MIN_SHIP_X)
	{
		spaceship_x++;
		*dir = 0;
	}
	else
		spaceship_x--;

	//Did new leftmost of rightmost part of ship hit a projectile
	if(ship_hit_projectile(spaceship_x-1) || ship_hit_projectile(spaceship_x+1))
	{
		if(--lives == 0)
			game_over = 1;
	}

	draw_ship(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, spaceship_x, SHIP_Y);
}

Xint8 projectile_hit_invader(int i, int j)
{
	int n;

	/* Check if projectile hit left, center or right part of invader */
	for(n = -1; n < 2; n++)
	{
		if(invaders_map[i+n][j])
		{
			invaders_map[i+n][j] = 0;
			erase_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i+n, j);
			invaders_num--;
			projectiles_of_ship_num[i]--;

			for(n = 0; n < 2000; n++)
			{
				draw_red(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j, 0);
				draw_red(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j, 0);

				erase_X(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);

				draw_X(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
				erase_X(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
			}

			xil_printf("\nPROJECTILE HIT INVADER\n\r");
			return 1;
		}
	}
	return 0;
}

Xint8 projectile_hit_projectile(int i, int j)
{
	if(projectiles_map[i][j])
	{
		projectiles_map[i][j] = 0;
		erase_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
		projectiles_of_invaders_num[i]--;
		projectiles_of_ship_num[i]--;
		return 1;
	}
	return 0;
}

void move_projectile_from_ship()
{
	int i, j, n;

	for(i = 1; i < MAX_PROJECTILES_X; i++)
	{
		n = 0;
		j = 0;

		while(n < projectiles_of_ship_num[i])
		{
			if(projectiles_map[i][j] == 1)
			{
				erase_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
				projectiles_map[i][j] = 0;

				if(j == 0)		//New position will be out of screen
					projectiles_of_ship_num[i]--;
				else			//New position is not out of screen
				{
					if(!projectile_hit_invader(i, j-1) && !projectile_hit_projectile(i, j-1))
					{
						projectiles_map[i][j-1] = 1;
						draw_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, (j-1));
						n++;
					}
				}
			}

			j++;
		}
	}
}

Xint8 projectile_hit_ship(int i, int j)
{
	int n;
	if(SHIP_Y == j)
	{
		for(n = -1; n < 2; n++)
		{
			if(spaceship_x+n == i)
			{
				projectiles_of_invaders_num[i]--;
				xil_printf("\nPROJECTILE HIT SHIP\n\r");
				return 1;
			}
		}
	}

	return 0;
}



Xint8 invader_hit_projectile(int i, int j)
{
	int n;

	if(projectiles_map[i][j] == 1)
	{
		projectiles_map[i][j] = 0;
		projectiles_of_ship_num[i]--;
		invaders_num--;
		erase_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);

		for(n = 0; n < 500; n++)
		{
			draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
			erase_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
		}

		xil_printf("\nINVADER HIT PROJECTILE\n\r");
		return 1;
	}
	else
		return 0;
}


Xint8 invader_hit_projectile_down(int i, int j)
{
	int n;
	/* Check if projectile hit left, center or right part of invader */
	for(n = -1; n < 2; n++)
	{
		if(projectiles_map[i+n][j] == 1)
		{
			projectiles_map[i+n][j] = 0;
			projectiles_of_ship_num[i+n]--;
			invaders_num--;
			erase_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i+n, j);

			for(n = 0; n < 500; n++)
			{
				draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
				erase_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, i, j);
			}

			xil_printf("\nINVADER HIT PROJECTILE DOWN! projectiles_num: %d\n\r", projectiles_of_ship_num[i+n]);
			return 1;
		}
	}
	return 0;
}



/* Draw projectile at current position of spaceship  */
void shoot_projectile_from_ship(Xuint8 spaceship_x)
{
    if(projectiles_map[spaceship_x][SHIP_Y-1] != 1)
    {
    	projectiles_of_ship_num[spaceship_x]++;

    	if(!projectile_hit_invader(spaceship_x, SHIP_Y-1) && !projectile_hit_projectile(spaceship_x, SHIP_Y-1))
    	{
			draw_projectile(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, spaceship_x, SHIP_Y - 1);
			projectiles_map[spaceship_x][SHIP_Y - 1] = 1;
    	}
    }
}



void generate_init_invaders_positions()
{
	int j;

	for(j = 0; j < INIT_ROWS + level - 1; j++)	//Generate positions for invaders
	{
		invaders_map[5][j+INVADER_INIT_ROW] = 1;
		invaders_map[10][j+INVADER_INIT_ROW] = 1;
		invaders_map[15][j+INVADER_INIT_ROW] = 1;
		invaders_map[20][j+INVADER_INIT_ROW] = 1;
		invaders_map[25][j+INVADER_INIT_ROW] = 1;
		invaders_map[30][j+INVADER_INIT_ROW] = 1;
		invaders_map[35][j+INVADER_INIT_ROW] = 1;
		invaders_map[40][j+INVADER_INIT_ROW] = 1;
		invaders_map[45][j+INVADER_INIT_ROW] = 1;
		invaders_map[50][j+INVADER_INIT_ROW] = 1;
		invaders_map[55][j+INVADER_INIT_ROW] = 1;
		invaders_map[60][j+INVADER_INIT_ROW] = 1;
		invaders_map[65][j+INVADER_INIT_ROW] = 1;
		invaders_map[70][j+INVADER_INIT_ROW] = 1;
		invaders_map[75][j+INVADER_INIT_ROW] = 1;
		invaders_map[80][j+INVADER_INIT_ROW] = 1;
		invaders_map[85][j+INVADER_INIT_ROW] = 1;
		invaders_map[90][j+INVADER_INIT_ROW] = 1;
		invaders_map[95][j+INVADER_INIT_ROW] = 1;
		invaders_map[100][j+INVADER_INIT_ROW] = 1;
		invaders_map[105][j+INVADER_INIT_ROW] = 1;

	}
}

void generate_init_blocks_position()
{
	projectiles_map[10][SHIP_Y-2] = 3;
	projectiles_map[30][SHIP_Y-2] = 3;
	projectiles_map[50][SHIP_Y-2] = 3;
	projectiles_map[70][SHIP_Y-2] = 3;
	projectiles_map[90][SHIP_Y-2] = 3;
}
