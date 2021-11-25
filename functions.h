#include <curses.h>
#include "colors.h"

#define KEY_SENTER 10
#define BACKSPACE 127

void create_text(int x, int y, char text[], int color);


bool init();

void start_level(char level[], char best_score[]);