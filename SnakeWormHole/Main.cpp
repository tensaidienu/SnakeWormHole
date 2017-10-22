#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "Objects.h"
#include "Snake.h"

//__________________________________________________________________________________________________________________________________
//----------------------------------------------------------GLOBAL VARIABLES--------------------------------------------------------
const int width_w = 800;
const int height_w = 600; 
const int FPS = 90;
int direction = 1;
int snakeSize = 30;
int speed = 1;
int walkCount = 0;
bool endGame = false;
bool draw = true;
int points = 0;
std::vector<Snake> snake;
std::vector<Snake>::iterator it;

enum TECLAS { UP, DOWN, LEFT, RIGHT, SPACE, ENTER };

//__________________________________________________________________________________________________________________________________
//-----------------------------------------------------------PROTOTYPES----------------------------------------------------------
void InitSnake (std::vector<Snake> &snake);

void DrawSnake (std::vector<Snake> &snake);

void MoveSnake(std::vector<Snake> &snake);

void InitWormhole (Wormhole &wormhole);

void DrawWormhole (Wormhole &wormhole);

void ReleaseWormhole (Wormhole &wormhole);

void SnakeEnterTheWormhole(Wormhole &wormhole, std::vector<Snake> &snake);

void GrowSnake(std::vector<Snake> &snake, int size);

void KillSnake(std::vector<Snake> &snake);

int main(){
	using namespace std;
	//__________________________________________________________________________________________________________________________________
	//----------------------------------------------------------LOCAL VARIABLES---------------------------------------------------------
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font15 = NULL;

	ALLEGRO_BITMAP  *displayIcon = NULL;

	bool end = false;
	bool keys[] = {false, false, false, false, false, false};

	//______________________________________________________________________________________________________________________________
	//----------------------------------------------------------INITIALIZATIONS-----------------------------------------------------
	Wormhole wormhole;
	snake.resize(1);
	it = snake.begin();

	ALLEGRO_DISPLAY *display = NULL;

	if(!al_init()){
		al_show_native_message_box(NULL,"ERROR!","ERROR","ALLEGRO WINDOW ERROR!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	//ALLEGRO_MONITOR_INFO	info;
	int res_x_comp,res_y_comp;
	//al_get_monitor_info(0, &info);
	//res_x_comp = info.x2 - info.x1;
	//res_y_comp = info.y2 - info.y1;
	res_x_comp = 800;
	res_y_comp = 600;

	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(res_x_comp,res_y_comp);

	if(!display){
		al_show_native_message_box(NULL,"ERROR!","ERROR","ALLEGRO DISPLAY ERROR!",NULL,ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_set_window_title(display, "SNAKE WORMHOLE");
	
	float red_x = res_x_comp / (float) width_w;
	float red_y = res_y_comp / (float) height_w;

	ALLEGRO_TRANSFORM transformar;
	al_identity_transform(&transformar);
	al_scale_transform(&transformar,red_x,red_y);
	al_use_transform(&transformar);

	//__________________________________________________________________________________________________________________________________
	//------------------------------------------------------ADDONS INITIALIZATIONS------------------------------------------------------
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	//__________________________________________________________________________________________________________________________________
	//------------------------------------------------CREATION OF QUEUE AND OTHER DEVICES-----------------------------------------------
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	font15 = al_load_font("kenvector_future.ttf", 15, NULL);

	al_hide_mouse_cursor(display);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_textf(font15,al_map_rgb(255,255,255),0,0,NULL,"SNAKE WORMEHOLE - LOADING.....");
	al_draw_text(font15,al_map_rgb(255,255,255),width_w / 2, height_w / 2, ALLEGRO_ALIGN_CENTER, "");
	al_flip_display();

	displayIcon = al_load_bitmap("imsnake.png");
	if (!displayIcon) {
		al_show_native_message_box(NULL, "WARNING!", "ERROR", "ERROR CANNOT LOAD ICON!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	al_set_display_icon(display, displayIcon);

	ALLEGRO_EVENT ev;

//__________________________________________________________________________________________________________________________________
//-------------------------------------------------------------REGISTER-------------------------------------------------------------
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
//__________________________________________________________________________________________________________________________________
//---------------------------------------------------------INITIAL FUNCTIONS--------------------------------------------------------
	srand(time(NULL));
	InitSnake(snake);
	InitWormhole(wormhole);

	//__________________________________________________________________________________________________________________________________
	//-------------------------------------------------------------MAIN LOOP------------------------------------------------------------
	al_start_timer(timer);	
	
	while (!end) {
		//SOME ERROR		
		al_wait_for_event(event_queue, &ev);

		//___________________________________________________________________________________________________________________________
		//-----------------------------------------------------EVENTS AND GAME LOGIC-------------------------------------------------
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			end = true;
		} else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					end = true;
					break;
				case ALLEGRO_KEY_UP:
					if (direction != 1) {
						direction = 0;
						snake[0].direction = 0;
					}
					keys[UP] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					if (direction != 0) {
						snake[0].direction = 1;
						direction = 1;
					}
					keys[DOWN] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					if (direction != 3) {
						snake[0].direction = 3;
						direction = 2;
					}
					keys[RIGHT] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					if (direction != 2) {
						snake[0].direction = 2;
						direction = 3;
					}
					keys[LEFT] = true;
					break;
				case ALLEGRO_KEY_ENTER:
					snake.clear();
					InitSnake(snake);
					snakeSize = 30;
					endGame = false;
					points = 0;
					break;
			}
		} /*else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = false;
					break;

				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = false;
					break;

				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = false;
					break;
			}
		} else if (ev.type == ALLEGRO_EVENT_TIMER) {
			draw = true;

			if (keys[UP]) {

			}
			if (keys[DOWN]) {

			}
			if (keys[RIGHT]) {

			}
			if (keys[LEFT]) {

			}
		}*/

		if (!endGame) {
			MoveSnake(snake);
			walkCount = speed;
			KillSnake(snake);
			if (!wormhole.active) {
				ReleaseWormhole(wormhole);
			}
			SnakeEnterTheWormhole(wormhole, snake);
		}
		
		//___________________________________________________________________________________________________________________________
		//-----------------------------------------------------DRAW-------------------------------------------------
		if (draw) {
			if (!endGame) {
				al_draw_textf(font15, al_map_rgb(255, 255, 255), width_w/2, 0, ALLEGRO_ALIGN_CENTER, "--------- / SCORE: %d", points);
			} else {
				//al_draw_textf(font15, al_map_rgb(255, 255, 255), width_w/2, 0, ALLEGRO_ALIGN_CENTER, "GAME OVER / SCORE: %d", points);
				al_draw_textf(font15, al_map_rgb(255, 255, 255), (width_w / 2), (height_w / 2) - 30, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
				al_draw_textf(font15, al_map_rgb(255, 255, 255), width_w / 2, (height_w / 2), ALLEGRO_ALIGN_CENTER, "Your Score: %d", points);
				al_draw_textf(font15, al_map_rgb(255, 255, 255), width_w / 2, (height_w / 2) + 30, ALLEGRO_ALIGN_CENTER, "Press ENTER to play again or ESC to exit");
			}
			DrawSnake(snake);
			DrawWormhole(wormhole);
		}

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		//___________________________________________________________________________________________________________________________
		//-----------------------------------------------------------END LOOP--------------------------------------------------------
	}
	
	//___________________________________________________________________
	//----------------------------Ending Game----------------------------
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font15);
	al_destroy_display(display);
	snake.clear();
	return 0;
}

//__________________________________________________________________________________________________________________________________
//----------------------------------------------------------BASIC FUNCTIONS---------------------------------------------------------

void InitSnake (std::vector<Snake> &snake){
	for (int x = 0; x < snakeSize; x++){
		Snake tempSnake;
		tempSnake.x = 100+x*2;
		tempSnake.y = 100;
		tempSnake.ID = SNAKE;
		tempSnake.life = 10;
		tempSnake.speed = 9;
		tempSnake.edge_x = 5;
		tempSnake.edge_y = 5;
		snake.push_back(tempSnake);
	}
}

void DrawSnake (std::vector<Snake> &snake){
	for (int x = 1; x < snakeSize; x++){
		//al_draw_filled_circle(snake[x].x, snake[x].y, 3, al_map_rgb(255,255,255));
		al_draw_filled_rectangle(snake[x].x, snake[x].y, snake[x].x + 3, snake[x].y + 3, al_map_rgb(255, 255, 255));
		//al_draw_line(snake[x].x, snake[x].y, snake[x].x + 3, snake[x].y + 3, al_map_rgb(255, 255, 255), 3);
	}
	//al_draw_filled_rectangle(nave.x - nave.borda_x, nave.y - nave.borda_y, nave.x + nave.borda_x,nave.y + nave.borda_y, al_map_rgba_f(.6,.6,.6,.6));
}

void MoveSnake(std::vector<Snake> &snake){
	while (walkCount > 0) {
		for (int x = 1; x < snakeSize; x++) {
			
			if (snake[x].direction == 3) {
				snake[x - 1].x = snake[x].x;
				snake[x - 1].y = snake[x].y;
				snake[x].x = snake[x].x + 2;
			}

			if (snake[x].direction == 2) {
				snake[x - 1].x = snake[x].x;
				snake[x - 1].y = snake[x].y;
				snake[x].x = snake[x].x - 2;
			}

			if (snake[x].direction == 1) {
				snake[x - 1].x = snake[x].x;
				snake[x - 1].y = snake[x].y;
				snake[x].y = snake[x].y + 2;
			}

			if (snake[x].direction == 0) {
				snake[x - 1].x = snake[x].x;
				snake[x - 1].y = snake[x].y;
				snake[x].y = snake[x].y - 2;
			}

			snake[x].direction = snake[0].direction;

			if (snake[x].x < 0) {
				snake[x].x = 800;
			}

			if (snake[x].x > 800) {
				snake[x].x = 0;
			}

			if (snake[x].y < 0) {
				snake[x].y = 600;
			}

			if (snake[x].y > 600) {
				snake[x].y = 0;
			}
		}
		walkCount--;
	}
}

void InitWormhole (Wormhole &wormhole){
	wormhole.ID = 01;
	wormhole.x = 0;
	wormhole.y = 0;
	wormhole.active = false;
}

void DrawWormhole (Wormhole &wormhole){
	al_draw_filled_circle(wormhole.x, wormhole.y, 5, al_map_rgb(255,255,0));
	al_draw_filled_circle(wormhole.x, wormhole.y, 3.7f, al_map_rgb(0, 0, 0));
	al_draw_filled_circle(wormhole.x, wormhole.y, 1, al_map_rgb(0, 0, 255));
	//al_draw_filled_rectangle(wormhole.x + 5, wormhole.y + 5, wormhole.x - 5, wormhole.y - 5, al_map_rgb(255, 255, 255));
}

void ReleaseWormhole (Wormhole &wormhole){
	wormhole.active = true;
	wormhole.x = rand() % 750 + 10;
	wormhole.y = rand() % 550 + 10;
}

void SnakeEnterTheWormhole(Wormhole &wormhole, std::vector<Snake> &snake){
	if(wormhole.active && snake[snakeSize-1].x > wormhole.x - 10 && snake[snakeSize-1].x < wormhole.x + 10 && snake[snakeSize-1].y > wormhole.y - 10 && snake[snakeSize-1].y < wormhole.y + 10){
			points++;
			wormhole.active = false;
			snakeSize +=15;
			GrowSnake(snake, 15);
			//speed++;
			//system("pause");
	}
}

void GrowSnake(std::vector<Snake> &snake, int size) {
	int randNum = rand() % 4;
	for (int x = 0; x < size; x++) {
		Snake tempSnake;
		//tempSnake.x = 100 + x * 2;
		//tempSnake.y = 100;
		tempSnake.ID = SNAKE;
		tempSnake.life = 10;
		tempSnake.speed = 9;
		tempSnake.edge_x = 5;
		tempSnake.edge_y = 5;
		switch (snake[0].direction) {
			case 0:
				snake[0].direction = 0;
				tempSnake.y = 600 + x * 2;
				break;
			case 1:
				snake[0].direction = 1;
				tempSnake.y = 0 + x * 2;
				break;
			case 2:
				snake[0].direction = 2;
				tempSnake.x = 800 + x * 2;
				break;
			case 3:
				snake[0].direction = 3;
				tempSnake.x = 0 + x * 2;
				break;
		}
		snake.push_back(tempSnake);
	}
}

void KillSnake(std::vector<Snake> &snake){
	for(int x=1; x < snakeSize - 10; x++){
		if (snake[snakeSize - 1].x > 0 && snake[snakeSize - 1].x < 800 && snake[snakeSize - 1].y > 0 && snake[snakeSize - 1].y < 600) {
			if (snake[snakeSize - 1].x == snake[x].x && snake[snakeSize - 1].y == snake[x].y) {
				endGame = true;
			}
		}
	}
}