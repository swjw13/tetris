#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define LEFT 75 	// Keyboard Input
#define RIGHT 77 
#define UP 72 
#define DOWN 80
#define SPACE 32
#define p 112
#define P 80
#define ESC 27 

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // Block State
#define CEILLING -1 
#define EMPTY 0
#define WALL 1
#define INACTIVE_BLOCK 2

#define MAIN_X 11	// Game borad Size
#define MAIN_Y 23 
#define MAIN_X_ADJ 3 
#define MAIN_Y_ADJ 1 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // Status info position

int STATUS_Y_GOAL; // Status info position

int STATUS_Y_SCORE;

int blocks[7][4][4][4] = {
	{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }
}; // Shape of block

int b_type; // block type
int b_rotation; // block rotation
int b_type_next; // new block

int main_org[MAIN_Y][MAIN_X]; 
int main_cpy[MAIN_Y][MAIN_X];

int bx, by; // x,y positionof moving block

int key; // keyboard input

int speed; // Game Speed

int new_block_on = 0; 
int crush_on = 0; 

void title(void);
void reset(void);
void reset_main(void);
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void); 
void new_block(void); 
void check_key(void); 

void winner(void);
void item(void);
int choose_item = 0;
int turn = 1;
int p1_score = 0;
int p1_item = 0;
int p2_score = 0;
int p2_item = 0;
int total_item = 0;
int used_item = 0;
int removed_lines = 0;
int tmp = 0;
int year, month, day, hour, min, sec;
char key_cpy[200] = { 0, };
int tem = 0;
FILE *itf;

int check_crush(int bx, int by, int rotation);
void drop_block(void); 
void move_block(int dir); 
void check_line(void);

void check_game_over(void); 
void pause(void); 

void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // hide cursor
void setcursortype(CURSOR_TYPE c) { // hide cursor
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
	int i;

	srand((unsigned)time(NULL)); 
	setcursortype(NOCURSOR); 
	title(); 
	reset(); 

	while (1) {
		if (turn == 61)
			winner();

		for (i = 0; i<5; i++) { 
			check_key();
			draw_main();

			Sleep(speed); 
			if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
		}
		drop_block();
		 
		check_game_over();
		if (new_block_on == 1)
		{
			if (tmp >= 1)
			{
				item();
			}
			turn += 1;
			draw_map();
			new_block();
			tmp = 0;
		}
	}
}

void title(void) {
	int x = 5;
	int y = 4;

	gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■");
	gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■");
	gotoxy(x, y + 2); printf("□□□■              □■  ■");
	gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□");
	gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□");
	gotoxy(x + 5, y + 2); printf("T E T R I S");
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
	gotoxy(x, y + 9); printf("  △   : Shift");
	gotoxy(x, y + 10); printf("◁  ▷ : Left / Right");
	gotoxy(x, y + 11); printf("  ▽   : Soft Drop");
	gotoxy(x, y + 12); printf("   P   : Pause");
	gotoxy(x, y + 13); printf("  ESC  : Quit");

	while (true)
	{
		if (kbhit()) break;
	}

	while (kbhit()) getch();

}

void reset(void) {
 
	struct tm *t;
	time_t timer;

	timer = time(NULL);
	t = localtime(&timer);

	year = t->tm_year + 1900;
	month = t->tm_mon + 1;
	day = t->tm_mday;
	hour = t->tm_hour;
	min = t->tm_min;
	sec = t->tm_sec;

	total_item = 0;
	used_item = 0;
	removed_lines = 0;

	p1_score = 0;
	p2_score = 0;
	key = 0;
	crush_on = 0;

	speed = 100;

	system("cls");
	reset_main(); 
	draw_map();
	draw_main(); 

	b_type_next = rand() % 7; 
	new_block();  

}

void reset_main(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) {  // Game board initialize
		for (j = 0; j<MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (j = 1; j<MAIN_X; j++) {
		main_org[3][j] = CEILLING;
	}
	for (i = 1; i<MAIN_Y - 1; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j<MAIN_X; j++) { 
		main_org[MAIN_Y - 1][j] = WALL;
	}
}

void reset_main_cpy(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) { 
			main_cpy[i][j] = 100;
		}
	}
}

void draw_map(void) { // Game Status
	int y = 3;
	STATUS_Y_SCORE = y + 9; 
	gotoxy(STATUS_X_ADJ, y); printf("player %d's turn ", 2 - (turn % 2));
	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 9); printf("player1 score: %6d", p1_score);
	gotoxy(STATUS_X_ADJ, y + 10); printf("player1 item : %6d", p1_item);
	gotoxy(STATUS_X_ADJ, y + 11); printf("player2 score: %6d", p2_score);
	gotoxy(STATUS_X_ADJ, y + 12); printf("player2 item : %6d", p2_item);
	gotoxy(STATUS_X_ADJ, y + 13); printf("    turn     : %6d", turn == 61?60:turn);
	gotoxy(STATUS_X_ADJ, y + 15); printf("  w        Shift       △   ");
	gotoxy(STATUS_X_ADJ, y + 16); printf("a   d  Left / Right  ◁  ▷");
	gotoxy(STATUS_X_ADJ, y + 17); printf("  s     Soft Drop      ▽  ");
	gotoxy(STATUS_X_ADJ, y + 19); printf(" P : Pause");
	gotoxy(STATUS_X_ADJ, y + 20); printf(" ESC : Quit");
}

void draw_main(void) {
	int i, j;

	for (j = 1; j<MAIN_X - 1; j++) {
		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
	}
	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) {
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_org[i][j]) {
				case EMPTY:
					printf("  ");
					break;
				case CEILLING:
					printf(". ");
					break;
				case WALL:
					printf("▩");
					break;
				case INACTIVE_BLOCK: 
					printf("□");
					break;
				case INACTIVE_BLOCK+1:
					printf("■");
					break;
				case ACTIVE_BLOCK:  
					if (turn % 2 == 1)
						printf("■");
					else
						printf("□");
					break;
				}
			}
		}
	}
	for (i = 0; i<MAIN_Y; i++) {  
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void new_block(void) {
	int i, j;

	bx = (MAIN_X / 2) - 1; 
	by = 0;
	
	if (tmp == 0)
		b_type = b_type_next;
	b_type_next = rand() % 7; 
	b_rotation = 0;
	new_block_on = 0; 

	for (i = 0; i<4; i++) { 
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
		}
	}
	for (i = 1; i<3; i++) { 
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				if (turn % 2 == 1)
					printf("□");
				else
					printf("■");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}
 
void check_key(void) {
	key = 0;  

	if (kbhit()) { 
		key = getch();
		if (turn % 2 == 1)
		{
			if (key == 224 || key == LEFT || key == RIGHT || key == DOWN || key == UP)
				return;
		}
		else
		{
			if (key == 'w' || key == 'a' || key == 's' || key == 'd')
				return;
		}
		if (key == 224) { // dirextion key
			do { key = getch(); } while (key == 224); 
			switch (key) {
			case LEFT:
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case RIGHT:
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN: 
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP: 
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
				break;
			}
		}

		else { // Not direction key
			switch (key) {
			case 'w':
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
				break;
			case 'a':
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case 's':
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case 'd':
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;

			case 'm':
				key_cpy[tem++] = 'm';
				tmp = 1;
				break;
			case 'f':
				key_cpy[tem++] = 'f';
				tmp = 1;
				break;

			case P:
			case p:
				pause();
				break;
			case ESC:
				system("cls");
				exit(0);
			}
		}
	}
	while (kbhit()) getch();
}

int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j]>0) return false;
		}
	}
	return true;
};

void drop_block(void) {
	int i, j;

	if (crush_on&&check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i<MAIN_Y; i++) { 
			for (j = 0; j<MAIN_X; j++) {
				if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK + (turn % 2);
			}
		}
		crush_on = 0;
		check_line();
		new_block_on = 1;    
		return; 
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--;
		break;

	case RIGHT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP: 
		for (i = 0; i<4; i++) {  
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; 
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100: 
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}
 
void check_line(void) {
	int i, j, k, l;
	int temp = 0;
	int y = 3;

	int block_amount; // block num of one line

	for (i = MAIN_Y - 2; i > 3;) {
		block_amount = 0;
		for (j = 1; j < MAIN_X - 1; j++) {
			if (main_org[i][j] > 0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) {
			if (turn % 2 == 1)
				p1_score += 1;
			else
				p2_score += 1;

			removed_lines += 1;
			temp += 1;

			for (k = i; k > 1; k--) { 
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
				}
			}
		}
		else i--;
	}
	if (temp >= 2)
	{
		if (turn % 2 == 1)
			p1_item += 1;
		else
			p2_item += 1;
	}

	gotoxy(STATUS_X_ADJ, y + 9); printf("player1 score: %6d", p1_score);
	gotoxy(STATUS_X_ADJ, y + 10); printf("player1 item : %6d", p1_item);
	gotoxy(STATUS_X_ADJ, y + 11); printf("player2 score: %6d", p2_score);
	gotoxy(STATUS_X_ADJ, y + 12); printf("player2 item : %6d", p2_item);

}

void check_game_over(void) {
	int i;

	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		if (main_org[3][i]>0) {  // end condition
			if (p1_score != p2_score)
			{

				gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
				gotoxy(x, y + 1); printf("▤                              ▤");
				gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
				gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
				gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
				gotoxy(x, y + 5); printf("▤   PLAYER1 SCORE: %6d      ▤", p1_score);
				gotoxy(x, y + 6); printf("▤   PLAYER2 SCORE: %6d      ▤", p2_score);
				gotoxy(x, y + 7); printf("▤                              ▤");
				gotoxy(x, y + 8); printf("▤      winner : %s             ▤", p1_score > p2_score ? "p1" : "p2");
				gotoxy(x, y + 9); printf("▤                              ▤");
				gotoxy(x, y + 10); printf("▤  Press any key to restart..  ▤");
				gotoxy(x, y + 11); printf("▤                              ▤");
				gotoxy(x, y + 12); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			}
			else
			{
				gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
				gotoxy(x, y + 1); printf("▤                              ▤");
				gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
				gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
				gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
				gotoxy(x, y + 5); printf("▤   PLAYER1 SCORE: %6d      ▤", p1_score);
				gotoxy(x, y + 6); printf("▤   PLAYER2 SCORE: %6d      ▤", p2_score);
				gotoxy(x, y + 7); printf("▤                              ▤");
				gotoxy(x, y + 8); printf("▤             DRAW             ▤");
				gotoxy(x, y + 9); printf("▤                              ▤");
				gotoxy(x, y + 10); printf("▤  Press any key to restart..  ▤");
				gotoxy(x, y + 11); printf("▤                              ▤");
				gotoxy(x, y + 12); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			}
			itf = fopen("record.txt","a");
			fprintf(itf, "%d년 %d월 %d일 %d시 %d분 %d초 / %d moved lines / %d obtain items / %d used items\n", year, month, day, hour, min, sec, removed_lines, total_item, used_item);

			fclose(itf);

			Sleep(1000);
			while (kbhit()) getch();
			key = getch();
			reset();
		}
	}
}

void pause(void) {
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤                              ▤");
		gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
		gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
		gotoxy(x, y + 6); printf("▤                              ▤");
		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}
	getch();

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void item(void) {
	int i, j;

	int x = 5;
	int y = 5;
	
	clock_t start = clock();

	if (key_cpy[0] == 'f')
	{
		if (p1_item <= 0)
		{
			tmp = 0;
			return;
		}
	}
	else if (key_cpy[0] == 'm')
	{
		if (p2_item <= 0) {
			tmp = 0;
			return;
		}
	}
	for (i = 1; i<MAIN_X - 2; i++) {
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤                              ▤");
		gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 3); printf("▤  |    * p%d's  I T E M *  |   ▤", key_cpy[0] == 'f' ? 1 : 2);
		gotoxy(x, y + 4); printf("▤  |                       |   ▤");
		gotoxy(x, y + 5); printf("▤  | choose block you like |   ▤");
		gotoxy(x, y + 6); printf("▤  +-----------------------+   ▤");
		gotoxy(x, y + 7); printf("▤  1.▤ 2.▤▤ 3. ▤   4.  ▤  ▤");
		gotoxy(x, y + 8); printf("▤    ▤   ▤▤  ▤▤▤   ▤▤  ▤");
		gotoxy(x, y + 9); printf("▤    ▤                  ▤    ▤");
		gotoxy(x, y + 10); printf("▤    ▤                        ▤");
		gotoxy(x, y + 11); printf("▤                              ▤");
		gotoxy(x, y + 12); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}

	while (1)
	{
		if (kbhit())
		{
			choose_item = getch();
			while (choose_item != '1' && choose_item != '2' && choose_item != '3' && choose_item != '4')
				choose_item = getch();
			start = clock();
			break;
		}
		if ((clock() - start) / CLOCKS_PER_SEC > 5.0f)
		{
			choose_item = 100; break;
		}
	}

	switch (choose_item) {
	case '1':
		b_type = 1;
		break;
	case '2':
		b_type = 0;
		break;
	case '3':
		b_type = 6;
		break;
	case '4':
		b_type = 2;
		break;
	}
	if (key_cpy[0] == 'f')
		p1_item -= 1;
	else if(key_cpy[0] == 'm')
		p2_item -= 1;

	tem = 0;

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();
	
}

void winner(void) {
	int i;

	int x = 5;
	int y = 5;
	gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); 
	gotoxy(x, y + 1); printf("▤                              ▤");
	gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
	gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
	gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
	gotoxy(x, y + 5); printf("▤   PLAYER1 SCORE: %6d      ▤", p1_score);
	gotoxy(x, y + 6); printf("▤   PLAYER2 SCORE: %6d      ▤", p2_score);
	gotoxy(x, y + 7); printf("▤                              ▤");
	gotoxy(x, y + 8); printf("▤      winner : %s             ▤", p1_score > p2_score ? "p1" : "p2");
	gotoxy(x, y + 9); printf("▤                              ▤");
	gotoxy(x, y + 10); printf("▤  Press any key to restart..  ▤");
	gotoxy(x, y + 11); printf("▤                              ▤");
	gotoxy(x, y + 12); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	itf = fopen("record.txt", "a");
	fprintf(itf, "%d년 %d월 %d일 %d시 %d분 %d초 / %d moved lines / %d obtain items / %d used items\n", year, month, day, hour, min, sec, removed_lines, total_item, used_item);

	fclose(itf);

	Sleep(1000);
	while (kbhit()) getch();
	key = getch();
	reset();
}