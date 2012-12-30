/* 
 * Tic-Tac-Toe: An implementation of the classic game.
 * Copyright (C) 2012 cyphar <public@cyphar.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>	/* fprintf(), printf() */
#include <stdarg.h>	/* va_list, va_*() */
#include <stdlib.h>	/* exit() */
#include <getopt.h>	/* getopt_long() */

#include <stdbool.h>	/* bool	*/
#include <termios.h>	/* getch() stuff */
#include <time.h>	/* time() */

#include "../include/ttt.h"

#define ANSI_COLOUR_GREEN	"\e[1;32m"
#define ANSI_COLOUR_BLUE	"\e[1;34m"
#define ANSI_COLOUR_RED		"\e[1;31m"
#define ANSI_COLOUR_RESET	"\e[0m"

bool nocr = false;
bool colour = true;
bool numon = true;
bool classicf = false;

void uholler(char *msg, ...) {
	va_list ap;

	va_start(ap, NULL);
	do {
		fprintf(stderr, "%s", msg);
	} while((msg = va_arg(ap, char *)));
	va_end(ap);

	fprintf(stderr, "\n");
} /* uholler() */

void ubail(char *msg, ...) {
	extern char *__progname;
	va_list ap;

	fprintf(stderr, "%s: ", __progname);
	
	va_start(ap, NULL);
	do {
		fprintf(stderr, "%s", msg);
	} while((msg = va_arg(ap, char *)));
	va_end(ap);

	fprintf(stderr, "\n");
	exit(1);
} /* ubail() */

char getch_(int echo) 
{
	char ch;
	static struct termios old, new;
	
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */

	ch = getchar();
	tcsetattr(0, TCSANOW, &old); /* reset terminal i/o settings */
	
	if(ch == 3) {
		printf("\n^C\n");
		exit(2);
	}
	return ch;
} /* getch_() */

#define getch() getch_(0)

int cyrand(unsigned int min, unsigned int max) { 
	/* Adapted from http://bit.ly/12Q9eQT */
	if(min == max) return min;	
	/* seed the random */
	srand(time(NULL));
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random) return cyrand(min, max);
	/* now guaranteed to be in [0, RAND_MAX) */
	int range	= max - min,
	remainder	= RAND_MAX % range,
	bucket		= RAND_MAX / range;
	/* There are range buckets, plus one smaller interval
	 *	within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
	 	return min + base_random/bucket;
	} else {
		return cyrand(min, max);
	}
} /* cyrand() */

char lowerch(char ch) {
	if (ch > 64 && ch < 91) {
		return ch + 32;
	}
	return ch;
} /* lowerch() */

void clearscr(void) {
	printf("\e[1;1H\e[2J");
} /* clearscr() */

void bake_args(int argc, char *argv[]) {
	char ch;
	int longindex;
	
	extern char *__progname;
	argv[0] = __progname;
	
	const struct option getopts[] = {
		{"nowait",	no_argument,	NULL,		'w'},
		{"nocolor",	no_argument,	NULL,		'c'},
		{"nonum",	no_argument,	NULL,		'n'},
		{"classic",	no_argument,	NULL,		'f'},
		{NULL,		0,		NULL,		0}
	};

	while((ch = getopt_long(argc, argv, "cfnw", getopts, &longindex)) != -1) {
		switch(ch) {
			case 'w':
				nocr = true;
				break;
			case 'c':
				colour = false;
				break;
			case 'n':
				numon = false;
				break;
			case 'f':
				classicf = true;
				break;
			default:
				break;
		}	
	};
} /* bake_args() */

void cleargame(void) {
	for(int i = 0; i < 9; i++) {
		thegame.board[i].position = i+1;
		thegame.board[i].state = NONE;
	}
	
	thegame.running = true;
	thegame.winner = NONE;

} /* cleargame() */

void initgame(void) {
	clearscr();
	do {
		printf("How many players? ");
		if(!nocr) {
			scanf("%d", &thegame.players);
			fflush(stdin);
		} else {
			thegame.players = getch_(1) - 48;
			if(thegame.players != -38) printf("\n");
		}
	} while(thegame.players > 2 || thegame.players < 0); /* < 0 is intentional, for a WarGames quote */

} /* initgame() */

void whatside(void) {
	char ch;

	clearscr();
	do {
		printf("X or O? ");
		if(!nocr) {
			scanf("%c", &ch);
			fflush(stdin);
		} else {
			ch = getch_(1);
			if(ch != 10) printf("\n");
		}
	} while(lowerch(ch) != 'x' && lowerch(ch) != 'o');
	player = (lowerch(ch) == 'x') ? X : O;
}

void wargames(void) {
	holler(	colour ? ANSI_COLOUR_BLUE : "",
		"\n",
		"A STRANGE GAME.\n",
		"THE ONLY WINNING MOVE IS NOT TO PLAY.\n",
		colour ? ANSI_COLOUR_RESET : ""); /* WarGames */
	exit(0);
} /* wargames() */

void printboard(bool nums) {
	int cur;

	/* Normal board layout:
	 *
	 * +---+---+---+
	 * | 1 | 2 | 3 |
	 * +---+---+---+
	 * | 4 | 5 | 6 |
	 * +---+---+---+
	 * | 7 | 8 | 9 |
	 * +---+---+---+
	 *
	 * "Classic" board layout:
	 *
	 *  1 | 2 | 3
	 * ---+---+---
	 *  4 | 5 | 6
	 * ---+---+---
	 *  7 | 8 | 9 
	 *
	 */
	clearscr();
	if(!classicf) printf("+---+---+---+\n");
	else printf("\n");
	for(int i = 0; i < 3; i++) {
		if(classicf) printf(" ");	
		for(int j = 0; j < 3; j++) {
			cur = (i * 3) + j;
			if(thegame.board[cur].state == NONE) {
				if(!classicf) {
					if(nums && numon) printf("| %d ", thegame.board[cur].position);
					else printf("|   ");
				} else {
					if(nums && numon) {
						if(j != 2) printf(" %d |", thegame.board[cur].position);
						else printf(" %d ", thegame.board[cur].position);
					} else {
						if(j != 2) printf("   |");
						else printf("    ");
					}
				}
			} else {
				if(!classicf) {
					if(colour) printf("| %s ", (thegame.board[cur].state == X) ? 
							ANSI_COLOUR_RED "X" ANSI_COLOUR_RESET : ANSI_COLOUR_GREEN "O" ANSI_COLOUR_RESET);
					else printf("| %s ", (thegame.board[cur].state == X) ? "X" : "O");
				} else {
					
					if(j != 2) { 
						if(colour) printf(" %s |", (thegame.board[cur].state == X) ? 
							ANSI_COLOUR_RED "X" ANSI_COLOUR_RESET : ANSI_COLOUR_GREEN "O" ANSI_COLOUR_RESET);
						else printf(" %s |", (thegame.board[cur].state == X) ? "X" : "O");
					} else {
						if(colour) printf(" %s ", (thegame.board[cur].state == X) ? 
							ANSI_COLOUR_RED "X" ANSI_COLOUR_RESET : ANSI_COLOUR_GREEN "O" ANSI_COLOUR_RESET);
						else printf(" %s ", (thegame.board[cur].state == X) ? "X" : "O");

					}

				}
			}
		}
		if(!classicf){
			printf("|\n");
			printf("+---+---+---+\n");
		} else {
			printf("\n");
			if(i != 2) printf(" ---+---+---\n");
		}
	}
	if(classicf) printf("\n");
} /* printboard() */

void makemove(enum value player) {
	int move;
	do {	
		printf("Make your move, %s: ", (player == X) ? "X" : "O");
		if(!nocr) {
			scanf("%d", &move);
			fflush(stdin);
		} else {
			move = getch_(1) - 48;
			if(move != -38) printf("\n");
		}
	} while(move > 9 || move < 1 || thegame.board[move - 1].state != NONE);

	thegame.board[move - 1].state = player;
} /* makemove() */

bool equal(int a, int b, int c) {
	/* Basically, if a == b && b == c then a == c */
	if(thegame.board[a].state == thegame.board[b].state &&
		thegame.board[b].state == thegame.board[c].state &&
		thegame.board[a].state != NONE) return true;
	else return false;

	return NULL; /* If you get up to this, you've got bigger problems */
} /* equal() */

bool checkwinner(void) {
	bool eog = true; /* Assume end of game */
	
	/* 
	 * 1 2 3
	 * 4 5 6
	 * 7 8 9
	 *
	 * ALL WINS:
	 * 1,2,3
	 * 4,5,6
	 * 7,8,9
	 * 1,4,7
	 * 2,5,8
	 * 3,6,9
	 * 1,5,9
	 * 3,5,7
	 */
	#define IEQUAL(a,b,c) if(equal(a,b,c)) thegame.winner = thegame.board[a].state
	#define EQUAL(a,b,c) else if(equal(a,b,c)) thegame.winner = thegame.board[a].state
	
	/* Horizontal */
	IEQUAL(0,1,2);
	EQUAL(3,4,5);
	EQUAL(6,7,8);
	
	/* Vertical */
	EQUAL(0,3,6);
	EQUAL(1,4,7);
	EQUAL(2,5,8);
	
	/* Diagonal */
	EQUAL(0,4,8);
	EQUAL(2,4,6);
	
	/* check for ties*/
	else for(int i = 0; i < 9 && eog; i++) if(thegame.board[i].state == NONE) eog = false;
	
	thegame.running = !eog;
	return eog;
} /* checkwinner() */

int oneaway(int a, int b, int c, enum value player) {
	if(thegame.board[b].state == thegame.board[c].state && thegame.board[a].state == NONE && thegame.board[b].state == player) return a;
	else if(thegame.board[c].state == thegame.board[a].state && thegame.board[b].state == NONE && thegame.board[c].state == player) return b;
	else if(thegame.board[a].state == thegame.board[b].state && thegame.board[c].state == NONE && thegame.board[a].state == player) return c;
	
	return -1;
} /* oneaway() */

void compmove(enum value ai) {
	int pos, tmp;
	holler("My move, I believe."); /* To tell the user the program is thinking */
	
	/* Do "intelligent" AI choice */
	
	/* 
	 * 1 2 3
	 * 4 5 6
	 * 7 8 9
	 *
	 * ALL WINS:
	 * 1,2,3
	 * 4,5,6
	 * 7,8,9
	 * 1,4,7
	 * 2,5,8
	 * 3,6,9
	 * 1,5,9
	 * 3,5,7
	 */
	#define IONE_WAY(a,b,c,d) if((tmp = oneaway(a,b,c,d)) != -1) pos = tmp
	#define ONE_WAY(a,b,c,d) else if((tmp = oneaway(a,b,c,d)) != -1) pos = tmp
	
	/* Always try to win, rather than block off player... */
	
	/* Horizontal */
	IONE_WAY(0,1,2,ai);
	ONE_WAY(3,4,5,ai);
	ONE_WAY(6,7,8,ai);
	
	/* Vertical */
	ONE_WAY(0,3,6,ai);
	ONE_WAY(1,4,7,ai);
	ONE_WAY(2,5,8,ai);
	
	/* Diagonal */
	ONE_WAY(0,4,8,ai);
	ONE_WAY(2,4,6,ai);

	/* ... if it can't win now, block off player ... */

	/* Horizontal */
	ONE_WAY(0,1,2,-ai);
	ONE_WAY(3,4,5,-ai);
	ONE_WAY(6,7,8,-ai);
	
	/* Vertical */
	ONE_WAY(0,3,6,-ai);
	ONE_WAY(1,4,7,-ai);
	ONE_WAY(2,5,8,-ai);
	
	/* Diagonal */
	ONE_WAY(0,4,8,-ai);
	ONE_WAY(2,4,6,-ai);
	
	else if(thegame.board[4].state == NONE) pos = 4; /* Always go for the centre square, if you can't block off the player or win */
	else {
		int min = 8, max = 0;
		for(int i = 0; i < 9; i++) if(thegame.board[i].state == NONE) max = i; /* Dial down the random range ... */
		for(int i = 8; i >= 0; i--) if(thegame.board[i].state == NONE) min = i; /* ... to make it more efficient */
		while(thegame.board[(pos = cyrand(min,max))].state != NONE);
	}
	
	/* My move, poor mortal */
	thegame.board[pos].state = ai;

}

void printwinner(void) {
	char yn;
	printboard(false);
	if(thegame.winner == NONE) printf("Stalemate.\n");
	else printf("%s wins.\n", (thegame.winner == X) ? "X" : "O");

	do {
		printf("Do you want to play again? ");
		if(!nocr) {
			scanf("%c", &yn);
			fflush(stdin);
		} else {
			yn = getch_(1);
			if(yn != 10) printf("\n");
		}
	} while(lowerch(yn) != 'y' && lowerch(yn) != 'n');

	if(yn == 'y') thegame.restart= true;
} /* printwinner() */
