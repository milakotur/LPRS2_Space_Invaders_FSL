
/*
 * SI_main.c
 *
 *  Created on: 16.06.2015.
 *      Authors: Standzi, Marko, Dva geja (Urosko i Milan GAY)
 */

#include "SI_logic.h"
#include "SI_interrupt.h"
#include <stdlib.h>

int ball_x, ball_y, ball_x_prev, ball_y_prev, move_ball=0;
int ball_angle=70;


void init_colors()
{
	/* Define colors */
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xCD2990);	//color 0 roza
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0xCD96CD);	//color 1 lila
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xff0000);	//color 2 crvena
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x1C, 0xEEEE00);	//color 3 zuta
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x20, 0x00ffff);	//color 4
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x24, 0xff00ff);	//color 5
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x28, 0xffff00);	//color 6
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x2C, 0x000000);	//color 7
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x30, 0xffffff);	//color 8
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x34, 0xff1493);	//color 9
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x38, 0xff6666);	//color 10 A
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x3C, 0x556b2f);	//color 11 B
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x40, 0x191970);	//color 12 C
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x44, 0xb8860b);	//color 13 D
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x48, 0xffe4c4);	//color 14 E
	VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x4C, 0xd2691e);	//color 15 F
}

void init_variables(Xuint8* spaceship_dir, Xuint8* invader_dir, Xuint8* invader_dir_chng, Xuint8* row)
{
	int i, j;

	/* Init all arrays and variables */
	for(i = 0; i < INIT_ROWS + level - 1; i++)
	{
		invader_dir[i] = RIGHT;
		invader_dir_chng[i] = 1;
		row[i] = i + INVADER_INIT_ROW;

		flag_row[i] = 0;
	}

	spaceship_speed_cnt = 0;
	projectil_speed = 0;
	spaceship_flag = 0;
	projectil_flag = 0;
	invader_speed = 0;
	shoot_flag = 0;
	invader_shoot_flag = 0;
	counter = 0;
	invaders_num = INIT_NUM + INIT_COLS*(level-1);
	game_over = 0;
	move_faster = 0,
	seed = 0;
	input = 0;

	if(level - 1 == 0)
	{
		*spaceship_dir = 0;
		spaceship_x = 7;
		lives = INIT_LIVES;
	}

	for(i = 0; i < MAX_PROJECTILES_X; i++)
	{
		projectiles_of_ship_num[i] = 0;
		projectiles_of_invaders_num[i] = 0;

		for(j = 0; j < MAX_PROJECTILES_Y; j++)
		{
			projectiles_map[i][j] = 0;
			invaders_map[i][j] = 0;
		}
	}
}

void init_draw(Xuint8* row)
{
	int i, j;

	draw_ship(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, spaceship_x, SHIP_Y);

	for(i = 0; i < INIT_ROWS + level - 1; i++)
	{
		for(j = 0; j < MAX_PROJECTILES_X; j++)
		{
			if(invaders_map[j][row[i]])
			{
				draw_brick(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, j, row[i], invaders_map[j][row[i]]);
			}
		}
	}

}

void clear_text_screen(Xuint32 BaseAddress)
{
   	int i;

    for (i = 0; i < 4800; i++)
    {
        VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, 0x20);
    }
}

void print_string(Xuint32 BaseAddress, char string_s[], int lenght)
{
    int i;

    for (i = 0; i < lenght; i++)
    {
        VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position + i*4, (string_s[i]-0x40));
    }
}

void set_cursor(Xuint32 new_value)
{
    cursor_position = new_value;
}

int num_len(int num)
{
	int len = 0;

	if(num == 0)
	{
		len++;
	}
	else
	{
		while(num > 0)
		{
			num /= 10;
			len++;
		}
	}

	return len;
}

void num_to_str(char *s, unsigned long bin, unsigned char n)
{
    s += n;
    *s = '\0';

    while (n--)
    {
        --s;
    	*s = (bin % 10) + '0';
        bin /= 10;
    }
}


int get_angle(int x, int y, int angle)
{
	int i, j;

	if(y<=480 && y>32 && x==2 && angle>180)
		return 540-angle;
	if(y<=480 && y>32 &&  x==2 && angle<=180)
		return 180-angle;
	if(y<=480 && y>32 && x==78 && angle<=90)
		return 180-angle;
	if(y<=480 && y>32 && x==78 && angle>270)
		return 540-angle;
	if(y==32 && angle<=90)
		return 360-angle;
	if(y==32 && angle>90)
		return 360-angle;
	if(y==448 && (x<=(spaceship_x+7)) && (x>=(spaceship_x-7)))
		return 360-angle;


	return angle;


}



//*********************SUDARANJE LOPTICE I CIGLE***********************

void sudaranje(int ball_x, int ball_y){

	int i, j;
	int k, x, y, m;


	for(i = 0; i < MAX_PROJECTILES_Y; i++){

			for(j = 0; j < MAX_PROJECTILES_X; j++){

				if(invaders_map[j][i]){

					//6 je precnik loptice 12 sirina cigle 32 sirina reda

					if((ball_y-6 == i*32+12) /*&& (ball_x >= (j*8-2)) && (ball_x <= (j*8+2))*/){


									for (y = 2 + i*32; y < 14 + i*32; y++){

										for(k = -2; k < 2; k++){
											//x = 5*8 + k;
											x = j*8 + k;
											m = y*(640/8) + x;
											PUT_TO_FSL(m, 0x00000000);
										}
									}
									/*	for (y = 2 + 5; y < 14 + 5; y++)
										{
											for(k = -2; k < 2; k++){
												x = 5*8 + k;
												i = y*(640/8) + x;
												PUT_TO_FSL(i, 0x22222222);
											}
										}
*/

					}
				}
			}
		}

}


//*********************************************************************


int main()
{
	/*************************/
	/* VARIABLES DECLARATION */
	/*************************/

	Xuint8 i, j, k1, k2, k3;
	Xuint8 spaceship_dir;
	Xuint8 invader_x;

	Xuint8* row;
	Xuint8* invader_dir;
	Xuint8* invader_dir_chng;

	/* Temp variables */
	int old_lives = 0, old_invaders_num = 0, old_level = -1;
	int score = 0, old_score = 0;
	char* str = NULL;

    /************************/
    /***** MAIN PROGRAM *****/
    /************************/

	init_platform();
	init_interrupt_controller();
	init_colors();

	clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);


	while(1)
	{
		clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

		/* Menu */
		VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0b01);    //text mode

		set_cursor(8115);

		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "PRESS KEY DOWN TO START", strlen("PRESS KEY DOWN TO START"));

		while(input & 0x01)	//wait for input from user
		{
			input = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
			seed++;
		}
		srand(seed);
		/********/

		VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0b11);	//display and text mode
		set_cursor(666);
		clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "SCORE", strlen("SCORE"));

		set_cursor(688);
		num_to_str(str, 0, num_len(0));
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));

		set_cursor(710);
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "LIVES", strlen("LIVES"));

		set_cursor(732);
		num_to_str(str, lives, num_len(lives));
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));

		set_cursor(751);
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "LEVEL", strlen("LEVEL"));

		game_over = 0;
		level = 0;
		print_level = 0;
		old_level = -1;
		score = 0;
		old_score = 0;
		j = 1;


		while(!game_over)
		{
			print_level++;

			if(level < 5)
			{
				level++;
			}

			old_score += score;

			row = (Xuint8*) malloc((INIT_ROWS+level-1)*sizeof(Xuint8));
			invader_dir = (Xuint8*) malloc((INIT_ROWS+level-1)*sizeof(Xuint8));
			invader_dir_chng = (Xuint8*) malloc((INIT_ROWS+level-1)*sizeof(Xuint8));

			init_variables(&spaceship_dir, invader_dir, invader_dir_chng, row);
			generate_init_invaders_positions();

			clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

			if(level > 1)
			{
				for(k1 = 0; k1 < 230; k1++)
					for(k2 = 0; k2 < 230; k2++)
						for(k3 = 0; k3 < 20; k3++)
						{
							set_cursor(6543);
							print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "NEXT LEVEL", 10);
						}
				clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
				set_cursor(666);
				clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
				print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "SCORE", strlen("SCORE"));

				set_cursor(688);
				num_to_str(str, 0, num_len(0));
				print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));

				set_cursor(710);
				print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "LIVES", strlen("LIVES"));

				set_cursor(732);
				num_to_str(str, lives, num_len(lives));
				print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));

				set_cursor(751);
				print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "LEVEL", strlen("LEVEL"));

			}

			init_draw(row);


			while(!((invaders_num == 0) || (game_over == 1)))

			{
				switch(input)
				{
					case LEFT_JOY:
							spaceship_dir = LEFT;
							spaceship_speed = 4;
							break;

					case RIGHT_JOY:
							spaceship_dir = RIGHT;
							spaceship_speed = 4;
							break;

					case MIDDLE_JOY:
							if(shoot_flag == 0)
							{
								ball_x=45;
								ball_y=190;
								move_ball=1;
							}
							break;
				}

				if(input != LEFT_JOY && input != RIGHT_JOY  ) //STISNUT NEKI JOY
				{
					spaceship_speed = 8;

				}


				if(spaceship_flag)	//time to move spaceship
				{
					move_spaceship(&spaceship_dir);
					spaceship_flag = 0;
				}

				if(projectil_flag && move_ball)
				{
					if(ball_angle==0)
												{
													ball_x++;
												}
												if(ball_angle==90)
												{
													ball_y--;
												}
												if(ball_angle==180)
												{
													ball_x--;
												}
												if(ball_angle==270)
												{
													ball_y++;
												}
												if(ball_angle>0 && ball_angle<90)
												{
													ball_x++;
													ball_y=-tan(ball_angle)+ball_y;
												}
												if(ball_angle>90 && ball_angle<180)
												{
													ball_x--;
													ball_y=-tan((180-ball_angle))+ball_y;
												}
												if(ball_angle>180 && ball_angle<270)
												{
													ball_x--;
													ball_y=ball_y+tan((ball_angle-180));
												}
												if(ball_angle>270 && ball_angle<360)
												{
													ball_x++;
													ball_y=ball_y+tan((360-ball_angle));
												}
												if(ball_y<17)
												{
													ball_y=17;
													draw_ball(ball_x,ball_y,ball_x_prev,ball_y_prev);
													sudaranje(ball_x, ball_y);
												}
												else
												{
													draw_ball(ball_x,ball_y,ball_x_prev,ball_y_prev);
													sudaranje(ball_x, ball_y);
												}
												ball_angle=get_angle(ball_x,ball_y,ball_angle);
												if(ball_angle<0) ball_angle+=360;
												if(ball_angle>360) ball_angle-=360;
												ball_x_prev=ball_x;
												if(ball_y>480)
												{
													move_ball=0;
												}
												ball_y_prev=ball_y;
												projectil_flag = 0;

				}




				if(old_invaders_num != invaders_num)
				{
					set_cursor(688);
					score = (INIT_NUM + INIT_COLS*(level-1) - invaders_num)*10;
					num_to_str(str, score + old_score, num_len(score + old_score));
					print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));
					old_invaders_num = invaders_num;

					if(score + old_score > 500*j)
					{
						j++;
						lives++;
					}
				}

				if(old_lives != lives)
				{
					set_cursor(732);
					num_to_str(str, lives, num_len(lives));
					print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));
					old_lives = lives;
				}

				if(old_level != print_level)
				{
					set_cursor(751 + 24);
					num_to_str(str, print_level, num_len(print_level));
					print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, strlen(str));
					old_level = print_level;
				}


			}

			free(row);
			free(invader_dir);
			free(invader_dir_chng);
		}

		VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0b01);    //text mode
		clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

		set_cursor(3660);
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "GAME OVER", 9);

		set_cursor(6543);
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, "SCORE", 5);

		set_cursor(6543 + 24);
		num_to_str(str, score + old_score, num_len(score + old_score));
		print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, str, num_len(score + old_score));
	}

    return 0;
}

