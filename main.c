#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<curses.h>
#include<time.h>
#include "colors.h"
#include "functions.h"


void main_menu();
void levels();


int main(int argv, char *argc[]){
	if(!init()) return 1;

	if(LINES < 24 || COLS < 85){
		create_text(COLS / 2 - 8, LINES / 2, "Too small screen", RED);
		getch();
		endwin();
		return 0;
	}		//85x24
	if(argv > 1) start_level(argc[1], "0 ");
	else main_menu();

	//getchar();
	endwin();
	return EXIT_SUCCESS;
}



void main_menu(){
	int position = 0;
	int key;
	do{
		clear();
		for(int i = 0; i <= COLS; i++){
			create_text(i, 2, "=",PASSIVE_BTN);
		}
		create_text(4, 2, "MAIN MENU", ACTIVE_BTN);

		create_text(1, 4, "PLAY", position == 0 ? ACTIVE_BTN : PASSIVE_BTN);
		create_text(1, 5, "EXIT", position == 1 ? ACTIVE_BTN : PASSIVE_BTN);

		key = getch();
		switch (key){
			case KEY_DOWN:
				if(position < 1) position++;
				break;
			case KEY_UP:
				if(position > 0) position--;
				break;
			case KEY_ENTER: case KEY_SENTER:
				if(position == 0) levels();
				if(position == 1) return;
				break;
		}

	}while(true);
}



void levels(){
	system("ls ./levels | grep \".map\" > .maps.txt");
	int position = 0;
	int key;
	do{
		clear();
		for(int i = 0; i <= COLS; i++){
			create_text(i, 2, "=",PASSIVE_BTN);
		}
		create_text(4, 2, "MAIN MENU>", PASSIVE_HEAD);
		create_text(14, 2, "LEVELS", ACTIVE_BTN);

		/*Nacitanie levelov*/
		FILE* flevels = fopen(".maps.txt", "r");
		if(flevels == NULL){
			create_text(1, 4, "There isn't any map!", RED);
			getch();
			return;
		}
		int count = 0;
		char levels[10][50];
		int t;
		do{
			t = fscanf(flevels, "%s\n", levels[count]);
			count++;
		}while(t && (count < 10));

		create_text(1, 4, "0) Tutorial", position == 0 ? ACTIVE_BTN : PASSIVE_BTN);
		create_text(1, 5, "1) Find it all!", position == 1 ? ACTIVE_BTN : PASSIVE_BTN);
		create_text(1, 7, "BACK", position == 2 ? ACTIVE_BTN : PASSIVE_BTN);

		key = getch();
		switch (key){
			case KEY_DOWN:
				if(position < 2) position++;
				break;
			case KEY_UP:
				if(position > 0) position--;
				break;
			case KEY_ENTER: case KEY_SENTER:
				//if(position == 0) levels();
				if(position == 2) return;
				else{
					FILE* score = fopen("score.txt", "r");
					if(score != NULL) {
						int r;
						for(int k = 0; k < position; k++){
							fscanf(score, "%d\n", &r);
						}
						char bscore[5];
						fscanf(score, "%s\n", bscore);
						start_level(levels[position], bscore);
					}else start_level(levels[position], "0 ");
					fclose(score);

				}
				break;
			case BACKSPACE: case KEY_BACKSPACE:
				return;
				break;
		}

	}while(true);
}
