#include <stdio.h>
#include <time.h>
#include "functions.h"
#include "colors.h"

int nanosleep(const struct timespec *req, struct timespec *rem);
void init_colors(){
	init_pair(PASSIVE_BTN, COLOR_YELLOW, COLOR_BLACK);
	init_pair(ACTIVE_BTN, COLOR_BLACK, COLOR_YELLOW);
	init_pair(PASSIVE_HEAD, 8, 0);
	init_pair(RED, 1, 2);
	init_pair(GRASS, 2, 2);
	init_pair(WALL, 7, 7);
	init_pair(TEXT, 4, 2);
	init_pair(MINE, 10, 2);
	init_pair(PLAYER, 5,2);
	init_pair(FINISH, 7, 0);
	init_pair(TREASURE, 11, 2);
	init_pair(HEAD, 15,0);
}


bool init(){
	initscr();
	cbreak();
    noecho();
    keypad(stdscr, TRUE);
	curs_set(FALSE);
	if(!has_colors())
	{	endwin();
		perror("Your terminal does not support color\n");
		return 0;
	}
	start_color();
	init_colors();
	return 1;
}



void create_text(int x, int y, char text[], int color){
	attron(COLOR_PAIR(color));
	mvprintw(y, x, "%s", text);
	attroff(COLOR_PAIR(color));
	refresh();
}

void create_char(int x, int y, char text, int color){
	attron(COLOR_PAIR(color));
	mvprintw(y, x, "%c", text);
	attroff(COLOR_PAIR(color));
	refresh();
}



void start_level(char level[], char best_score[]){
	//Nacitanie levelu
	char level1[50];
	sprintf(level1, "./levels/%s", level);
	FILE* fp = fopen(level1, "r");
	if(fp == NULL){
		perror("Missing or incorrect file.");
		return;
	}
	clear();
	create_text(0,0, "SCORE:", HEAD);
	create_text(0,1, "0 ", HEAD);
	create_text(10,0, "BEST SCORE:", HEAD);
	create_text(10,1, best_score, HEAD);

	int field_size[2];

	fscanf(fp, "%d %d\n", &field_size[0], &field_size[1]);
	char field[field_size[0]][field_size[1]];
	for(int i = 0; i < field_size[0]; i++){
		fgets(field[i], field_size[1]+2, fp);
	}
	
	for(int i = 0; i < field_size[0];i++){
		for(int j = 0; j < field_size[1]; j++){
			int color;
			switch(field[i][j]){
				case '=':
					color = WALL;
					break;
				case ' ':
					color = GRASS;
					break;
				case '@':
					color = MINE;
					break;
				case '#':
					color = FINISH;
					break;
				case '$':
					color = TREASURE;
					break;
				default:
					color = TEXT;
					break;
			}

			create_char(j, i+2, field[i][j], color);
			if((field[i][j] >= 'a' && field[i][j] <= 'z' ) || (field[i][j] >= 'A' && field[i][j] <= 'Z' )) field[i][j] = ' ';
			if(field[i][j] == '!') field[i][j] = ' ';
			
		}
		struct timespec ts = {
		.tv_sec = 0,                    // nr of secs
		.tv_nsec = 0.1 * 1000000000L  // nr of nanosecs
		};

		nanosleep(&ts, NULL);
	}
	create_text(4, field_size[0] + 3, "Press any key to start....", PASSIVE_HEAD);
	int x, y;	//pozicia hraca
	fscanf(fp, "\nP %d %d", &x, &y);
	fclose(fp);
	getch();
	if(x == -1 && y == -1) return;

	//Pridanie hraca
	field[y][x] = 'A';
	int score = 0;
	//int time_passed = 0;
	//Game Loop
	int key;
	char last_letter = ' ';
	nodelay(stdscr, TRUE);
	clear();
	while(last_letter != '#' && last_letter != '@'){
		
		for(int i = 0; i < field_size[0];i++){
			for(int j = 0; j < field_size[1]; j++){
				int color;
				switch(field[i][j]){
					case '=':
						color = WALL;
						break;
					case ' ':
						color = GRASS;
						break;
					case '@':
						color = MINE;
						break;
					case '#':
						color = FINISH;
						break;
					case '$':
						color = TREASURE;
						break;
					default:
						color = PLAYER;
						break;
				}

				create_char(j, i+2, field[i][j], color);
			}
		}
		key = getch();

		switch(key){
			case 'w': case 'W':
				if(field[y - 1][x] != '='){
					y--;
					last_letter = field[y][x];
					field[y+1][x] = ' ';
					field[y][x] = 'A';
					if(last_letter == '$') score += 500;
				}
			break;
			case 's': case 'S':
				if(field[y + 1][x] != '='){
					y++;
					last_letter = field[y][x];
					field[y-1][x] = ' ';
					field[y][x] = 'A';
					if(last_letter == '$') score += 500;
				}
			break;
			case 'a': case 'A':
				if(field[y][x - 1] != '='){
					x--;
					last_letter = field[y][x];
					field[y][x+1] = ' ';
					field[y][x] = 'A';
					if(last_letter == '$') score += 500;
				}
			break;
			case 'd': case 'D':
				if(field[y][x + 1] != '='){
					x++;
					last_letter = field[y][x];
					field[y][x-1] = ' ';
					field[y][x] = 'A';
					if(last_letter == '$') score += 500;
				}
			break;

		}
		if(last_letter == '#') score += 1500;
		//time_passed++;
		char scote_str[10];
		sprintf(scote_str, "%d ", score);
		create_text(0,0, "SCORE:", HEAD);
		create_text(0,1, scote_str, HEAD);
		create_text(10,0, "BEST SCORE:", HEAD);
		create_text(10,1, best_score, HEAD);
		/*truct timespec ts = {
		.tv_sec = 0,                    // nr of secs
		.tv_nsec = 0.1 * 1000000000L  // nr of nanosecs
		};

		nanosleep(&ts, NULL);*/
	}
	nodelay(stdscr, FALSE);

	if(last_letter == '#'){
		create_text((field_size[1]/2) - 9, field_size[0]/2, "Congrats! You won!", PASSIVE_BTN);
		create_text((field_size[1]/2) - 15, 1+ field_size[0]/2, "Press any key to continue....", PASSIVE_HEAD);
		getchar();
		return;
	}else{
		create_text((field_size[1]/2) - 13, field_size[0]/2, "You BLEW UP! Try it again.", PASSIVE_BTN);
		create_text((field_size[1]/2) - 15, 1+ field_size[0]/2, "Press any key to continue....", PASSIVE_HEAD);
		getchar();
		return;
	}
	return;

}