#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>




#define scr_width 320
#define scr_height 240
#define scr_BPP 2 //16 bits per pixel(in bytes)
#define whole_screen scr_width*scr_height*scr_BPP

#define BLOCK_SIZE 10

#define BLOCK_SIZE_X (scr_width/BLOCK_SIZE)
#define BLOCK_SIZE_Y (scr_height/BLOCK_SIZE)
#define MAX_LENGTH 30


struct fb_copyarea screen;
int fbfd = 0, GPDD = 0; //Framebuffer function driver and GAMEPAD device driver


uint16_t *framebuffer;
typedef union {
		struct{ 
			uint16_t B : 5;
			uint16_t G : 6;
			uint16_t R : 5;
		};
		uint16_t toint;
} Color;


enum direction{
	NONE 	=  0,
	UP 	 	=  1,
	DOWN 	= -1,
	RIGHT 	=  2,
	LEFT 	= -2
};

void flush_fb(void){
	ioctl(fbfd, 0x4680, &screen);
}


Color color(uint8_t r, uint8_t g, uint8_t b) {
	Color col;
	if(r >= 0 && r < 32 && g >= 0 && g < 64 && b >= 0 && b < 32){
		col.R = r;
		col.G = g;
		col.B = b;
	} else {
		col.R = 0;
		col.G = 0;
		col.B = 0;
		printf("Invalid color value supplied: (R: %d, G: %d, B: %d), max (31, 63, 31)", r, g, b);
	}
	return col;
}

typedef union{
		struct{
			uint8_t x;
			uint8_t y;
		};
		uint16_t coordinates;
}coordinate;

coordinate Coordinate(uint8_t x, uint8_t y) {
	coordinate coord;
	if(x >= 0 && x<BLOCK_SIZE_X && y >= 0 && y<BLOCK_SIZE_Y){
		coord.x = x;
		coord.y = y;
	}
	else
	{
		coord.x = 0;
		coord.y = 0;
		printf("invalid coordinat, your value x %d, y %d, max x: %d, max y: %d",x,y,BLOCK_SIZE_X, BLOCK_SIZE_Y);
	}
	return coord;
}

typedef struct {
	uint8_t lenght;
	coordinate pos[MAX_LENGTH];
	Color color;
	enum direction dir;
} snake;


typedef struct {
	coordinate pos;
	Color color;
} Item;



void error_exit(void)
{
	close(fbfd);
	close(GPDD);
	exit(EXIT_SUCCESS);
}

void exit_clean(void)
{
	error_exit();
}

void draw_pixel(uint16_t x, uint16_t y, Color color){
	if(x < scr_width && x >= 0 && y < scr_height && y >=0){
		framebuffer[ x+ y*scr_width ] = color.toint;
	}else {
		printf("invalid pixel position: (%d,%d), max %d x %d", x,y,(scr_width -1), (scr_height -1));
		}

}

void draw_block(uint8_t x, uint8_t y, Color color)
{
	uint8_t i, j;
	for(i = 0; i < BLOCK_SIZE; i++)
	{
		for(j = 0; j < BLOCK_SIZE; j++)
		{	
			draw_pixel(x*BLOCK_SIZE + i, y*BLOCK_SIZE +j, color);
		}
	}
}

void clear_screen()
{
	memset(framebuffer, 0, whole_screen);
	flush_fb();
}
snake Snake;

int crash(coordinate point)
{
	if(point.x <0 || point.x >= BLOCK_SIZE_X || point.y < 0 || point.y >=BLOCK_SIZE_Y)
	{
		return 1; // we have a crash!
	}
	uint8_t i;
	for(i=0; i < Snake.lenght; i++)
	{
		if(Snake.pos[i].coordinates == point.coordinates)
		{
			return 1; // we have a crash!
		}
	}
	return 0; // No crash!
}


Item Fruit;
Color col_background;

void place_fruit()
{
	srand(time(NULL));
	
	Fruit.pos.x = random() % BLOCK_SIZE_X;
	Fruit.pos.y = random() % BLOCK_SIZE_Y;
	if(crash(Fruit.pos))
	{
		printf("Position taken, placing fruit again!");
		place_fruit();
	}
	else
	{
		draw_block(Fruit.pos.x, Fruit.pos.y, Fruit.color);
	}
}

void move_snake(enum direction dir)
{
	coordinate current_pos;
	if(dir == -Snake.dir || dir == NONE) return;
	Snake.dir = dir;
	current_pos = Snake.pos[0];
	uint8_t i;
	switch(dir)
	{
		case UP:
			current_pos.y--;
			break;
		case DOWN:
			current_pos.y++;
			break;
		case LEFT:
			current_pos.x--;
			break;
		case RIGHT:
			current_pos.x++;
			break;
		default:
			return;
	}
	if(crash(current_pos))
	{
		printf("Snake Crashed! You lost");
		clear_screen();
		for(i =0; i< Snake.lenght; i++)
		{
			draw_block(Snake.pos[i].x, Snake.pos[i].y, Snake.color);
		}
		flush_fb();
		exit_clean();
	}
	
	for(i = Snake.lenght; i >0; i--)
	{
		Snake.pos[i] = Snake.pos[i-1];
	}
	Snake.pos[0] = current_pos;
	
	if(current_pos.coordinates == Fruit.pos.coordinates)
	{
		Snake.lenght++;
		if(Snake.lenght >= MAX_LENGTH)
		{
			printf("Your to good! Snake is at max lenght!");
			memset(framebuffer, 0xFFF0, whole_screen);
			for(i = 0; i < Snake.lenght; i++)
			{
				draw_block(Snake.pos[i].x, Snake.pos[i].y, Snake.color);
			}
			flush_fb();
			exit_clean();
		}
		place_fruit();
		
	}
	else
	{
		draw_block(Snake.pos[Snake.lenght].x, Snake.pos[Snake.lenght].y, col_background);
	}
	draw_block(current_pos.x, current_pos.y, Snake.color);
}





void start_game(void)
{
	Snake.lenght = 1;
	Snake.dir = NONE;
	Snake.pos[0].x = BLOCK_SIZE_X/2;
	Snake.pos[0].y = BLOCK_SIZE_Y/2;
	Snake.color = color(0,63,0);
	Fruit.color = color(1,20,8);
	draw_block(Snake.pos[0].x, Snake.pos[0].y, Snake.color);
	place_fruit();
	flush_fb();
}
	
int8_t last_dir = 0;
void button_push(int signal)
{
	char button, input_buffer;
	
	if(read(GPDD,&input_buffer, 1) != -1){
		button = input_buffer & 0x0F; //Uses button 1-4
		switch(button){
			case 1:
				move_snake(LEFT);
				last_dir = LEFT;
				break;
			case 2:
				move_snake(UP);
				last_dir = UP;
				break;
			case 4:
				move_snake(RIGHT);
				last_dir = RIGHT;
				break;
			case 8:
				move_snake(DOWN);
				last_dir = DOWN;
				break;
			default:
				move_snake(last_dir);
		}
		//if(input_buffer)
		flush_fb();
	}
	else{
		printf("failed to read buttons!");
	}

}



int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	printf("push one of the buttons 1 - 4 to start! \n");
	
	screen.dx = 0;
	screen.dy = 0;
	screen.width = scr_width;
	screen.height = scr_height;
	
	fbfd = open("/dev/fb0", O_RDWR);
	if(fbfd == -1){
		printf("can't open frambuffer driver!!!\n");
		}
		
	int screensize = scr_width * scr_height * scr_BPP;
	framebuffer = (uint16_t*) mmap(NULL, screensize, PROT_READ| PROT_WRITE, MAP_SHARED, fbfd,0);
	if(framebuffer == MAP_FAILED)
	{
		printf("Error mapping frammebuffer!");
		error_exit();
	}
	clear_screen();
	
	GPDD = open("/dev/gamepad", O_RDWR);
	if(GPDD == -1){
		printf("Can't open gamepad driver!! Exiting... \n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGIO, &button_push) == SIG_ERR){
		printf("can't set up signal handler!");
		error_exit();
	}	
	if(fcntl(GPDD, F_SETOWN, getpid()) == -1){
		printf("Failed to set owner of device!");
		error_exit();
	}
	long flags = fcntl(GPDD, F_GETFL);
	if(fcntl(GPDD, F_SETFL, flags| FASYNC) ==-1){
		printf("error setting flags!");
		error_exit();
	}
	
	col_background = color(0,0,0);
	
	start_game();
	while(1)
	{
		usleep(500000);
		button_push(0);
		
		//pause();	
	
	}
	munmap(framebuffer, screensize);
	exit_clean();
	
	exit(EXIT_SUCCESS);
}
