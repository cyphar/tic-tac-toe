/* 
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (C) 2012 Cyphar <public@cyphar.com>
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
#include <stdlib.h>	/* exit(), srand(), rand() */
#include <getopt.h>	/* getopt_long() */

#include <stdbool.h>	/* bool	*/
#include <termios.h>	/* getch() stuff */

#include "ttt.h"

#define USAGE	"Usage: %s [-cfnpw] [-hvl]\n", __progname

#define SYNTAX	"\n" \
		"Options:\n" \
		"  -c, --nocolor         Disable colours\n" \
		"  -f, --classic         Use the \"classic\" board layout\n" \
		"  -n, --nonum           Disable numbers\n" \
		"  -p, --printmoves      Print a list of moves at end of game\n" \
		"  -w, --nowait          Disable input buffering\n" \
		"\n" \
		"  -h, --help            Show this help screen\n" \
		"  -v, --version         Show version information\n" \
		"  -l, --license         Show license information\n" \
		"\n" \
		"Report bugs to <public@cyphar.com>.\n"

#define LICENSE	"Tic-Tac-Toe: A heuristic implementation of the classic game.\n" \
		"Copyright (C) 2012 Cyphar <public@cyphar.com>\n" \
		"\n" \
		"This program is free software: you can redistribute it and/or modify\n" \
		"it under the terms of the GNU General Public License as published by\n" \
		"the Free Software Foundation, either version 3 of the License, or\n" \
		"(at your option) any later version.\n" \
		"\n" \
		"This program is distributed in the hope that it will be useful,\n" \
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
		"GNU General Public License for more details.\n" \
		"\n" \
		"You should have received a copy of the GNU General Public License\n" \
		"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"

#define ANSI_COLOUR_GREEN	"\e[1;32m"
#define WARGAMES		"\e[1;96m"
#define ANSI_COLOUR_RED		"\e[1;31m"
#define ANSI_COLOUR_RESET	"\e[0m"

bool nocr = false;
bool colour = true;
bool numon = true;
bool classicf = false;
bool print = false;


void usage(void){
	extern char *__progname;
	printf(USAGE);
} /* usage() */

void syntax(void) {
	usage();
	printf(SYNTAX);
} /* syntax() */

void version(void) {
	printf("Tic-Tac-Toe (version %s) by Cyphar\n", TTT_VERSION);
} /* version() */

void license(void) {
	printf(LICENSE);
} /* license() */


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
	int r;
	if(min == max) return min;	
	
	r = (int) (1.0 * (max + 1.0) * rand() / (RAND_MAX + min + 0.0));
	
	return r;		
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

void inflush(void) {
	while(getchar() != '\n');
} /* inflush() */

void bake_args(int argc, char *argv[]) {
	char ch;
	int longindex;
	
	extern char *__progname;
	argv[0] = __progname;
	
	const struct option getopts[] = {
		/* Game Flags*/
		{"nowait",	no_argument,	NULL,	'w'},
		{"nocolor",	no_argument,	NULL,	'c'},
		{"nonum",	no_argument,	NULL,	'n'},
		{"classic",	no_argument,	NULL,	'f'},
		{"printmoves",	no_argument,	NULL,	'p'},
		
		/* Information */
		{"help",	no_argument,	NULL,	'h'},
		{"version",	no_argument,	NULL,	'v'},
		{"license",	no_argument,	NULL,	'l'},
		
		/* Terminate the array */
		{NULL,		0,		NULL,	0}
	};

	while((ch = getopt_long(argc, argv, "cfnpw hvl", getopts, &longindex)) != -1) {
		switch(ch) {
			/* Game Flags*/
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
			case 'p':
				print = true;
				break;

			/* Information */
			case 'h':
				syntax();
				exit(0);
				break;
			case 'v':
				version();
				exit(0);
				break;
			case 'l':
				license();
				exit(0);
				break;

			/* Catch-alls */
			case '?':
				usage();
				exit(1);
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
	
	for(int i = 0; i < 4; i++) {
		thegame.xmoves[i] = -1;
		thegame.omoves[i] = -1;
	}
	
	thegame.xmoves[4] = -1;
	
	thegame.running = true;
	thegame.winner = NONE;
	thegame.moves = 0;

} /* cleargame() */

void initgame(void) {
	clearscr();
	
	version();
	printf("--\n");
	
	do {
		if(!nocr) {
			do {
				printf("How many players? ");
			} while((thegame.players = getchar() - 48) == -38);
			inflush();
		} else {
			printf("How many players? ");
			thegame.players = getch_(1) - 48;
			if(thegame.players != -38) printf("\n");
		}
	} while(thegame.players > 2 || thegame.players < 0); /* < 0 is intentional, for a WarGames quote */

} /* initgame() */

void whatside(void) {
	char ch;

	clearscr();
	do {
		if(!nocr) {
			do {
				printf("X or O? ");
			} while((ch = getchar()) == 10);
			inflush();
		} else {
			printf("X or O? ");
			ch = getch_(1);
			if(ch != 10) printf("\n");
		}
	} while(lowerch(ch) != 'x' && lowerch(ch) != 'o');
	player = (lowerch(ch) == 'x') ? X : O;
}

void wargames(void) {
	if(thegame.winner == NONE) {
		holler(	colour ? WARGAMES : "",
			"\n",
			"A STRANGE GAME.\n",
			"THE ONLY WINNING MOVE IS NOT TO PLAY.\n",
			colour ? ANSI_COLOUR_RESET : ""); /* WarGames */
	} else {
		holler( colour ? WARGAMES : "",
			"\n",
			"A STRANGE GAME.\n",
			"THE ONLY WINNING MOVE IS ... WAIT, WHAT?\n",
			colour ? ANSI_COLOUR_RESET : "");
	}
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
		if(!nocr) {
			do {
				printf("Make your move, %s: ", (player == X) ? "X" : "O");
			} while((move = getchar() - 48) == -38);
			inflush();
		} else {
			printf("Make your move, %s: ", (player == X) ? "X" : "O");
			move = getch_(1) - 48;
			if(move != -38) printf("\n");
		}
	} while(move > 9 || move < 1 || thegame.board[move - 1].state != NONE);

	thegame.board[move - 1].state = player;
	
	if(player == X) thegame.xmoves[++thegame.moves / 2] = move;
	else thegame.omoves[thegame.moves++ / 2] = move;
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
	int pos, tmp, min, max;
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
	
	/* Take inspiration from http://bit.ly/ZRxOCg */
	
	/* Second player should go for the centre square first, if player one didn't */
	else if((ai == O || (ai == X && thegame.moves > 1)) && thegame.board[4].state == NONE) pos = 4;
	
	/* Corners are a far better bet for the first player's first move than edges or the centre ...
	 * ... and is a far better bet for player two (if the centre has been already taken) */
	else if(thegame.moves <= 1 &&
		(thegame.board[0].state == NONE || 
		thegame.board[2].state == NONE ||
		thegame.board[6].state == NONE ||
		thegame.board[8].state == NONE)) while(thegame.board[(pos = cyrand(0,4) * 2)].state != NONE && pos != 4);
	
	/* Player two MUST go for an edge as their second move if the other player has ...
	 * ... gone for at least ONE corner (or else they can be beaten easily, by only attacking corners or edges) */
	else if(((ai == O && thegame.moves == 3) || (ai == X && thegame.moves == 4)) &&
			(thegame.board[1].state == NONE ||
			thegame.board[3].state == NONE ||
			thegame.board[5].state == NONE ||
			thegame.board[7].state == NONE) &&
			(thegame.board[0].state == -ai ||
			 thegame.board[2].state == -ai ||
			 thegame.board[6].state == -ai ||
			 thegame.board[8].state == -ai)) while(thegame.board[(pos = (cyrand(0,3) * 2) + 1)].state != NONE);

	else if(((ai == O && thegame.moves == 3) || (ai == X && thegame.moves == 4)) &&
			(thegame.board[1].state == NONE ||
			thegame.board[3].state == NONE ||
			thegame.board[5].state == NONE ||
			thegame.board[7].state == NONE)) while(thegame.board[(pos = cyrand(0,4) * 2)].state != NONE);

	/* TODO: Make it go for the corner between the two edges, not a random corner */
	
	/* Have a hand in every pie - ensure that AI has made a move in every column ... */
	else if(thegame.board[0].state != ai && 
		thegame.board[3].state != ai && 
		thegame.board[6].state != ai) while(thegame.board[(pos = cyrand(0,2) * 3)].state != NONE);
	
	else if(thegame.board[1].state != ai && 
		thegame.board[4].state != ai && 
		thegame.board[7].state != ai) while(thegame.board[(pos = 1 + cyrand(0,2) * 3)].state != NONE);
	
	else if(thegame.board[2].state != ai && 
		thegame.board[5].state != ai && 
		thegame.board[8].state != ai) while(thegame.board[(pos = 2 + cyrand(0,2) * 3)].state != NONE);

	/* ... and every row. */
	else if(thegame.board[0].state != ai && 
		thegame.board[1].state != ai && 
		thegame.board[2].state != ai) while(thegame.board[(pos = cyrand(0,2))].state != NONE);
	
	else if(thegame.board[3].state != ai && 
		thegame.board[4].state != ai && 
		thegame.board[5].state != ai) while(thegame.board[(pos = 3 + cyrand(0,2))].state != NONE);
	
	else if(thegame.board[6].state != ai && 
		thegame.board[7].state != ai && 
		thegame.board[8].state != ai) while(thegame.board[(pos = 6 + cyrand(0,2))].state != NONE);

	/* Worst case scenario - take the novice approach */
	else {
		min = 8, max = 0;
		for(int i = 0; i < 9; i++) if(thegame.board[i].state == NONE) max = i; /* Dial down the random range ... */
		for(int i = 8; i >= 0; i--) if(thegame.board[i].state == NONE) min = i; /* ... to make it more efficient */
		
		while(thegame.board[(pos = cyrand(min,max))].state != NONE);
	}
	
	/* My move, poor mortal */
	thegame.board[pos].state = ai;
	if(ai == X) thegame.xmoves[++thegame.moves / 2] = pos + 1;
	else thegame.omoves[thegame.moves++ / 2] = pos + 1;

}

void restart(void) {
	char yn;
	do {
		if(!nocr) {
			do {
				printf("Do you want to play again? ");
			} while((yn = getchar()) == 10);
			inflush();
		} else {
			printf("Do you want to play again? ");
			yn = getch_(1);
			if(yn != 10) printf("\n");
		}
	} while(lowerch(yn) != 'y' && lowerch(yn) != 'n');

	if(lowerch(yn) == 'y') thegame.restart= true;
}

void printwinner(void) {
	printboard(false);
	if(thegame.winner == NONE) printf("Stalemate.\n");
	else printf("%s wins.\n", (thegame.winner == X) ? "X" : "O");
	
	if(print) {
		printf("\nX moves: [%d, %d, %d, %d, %d]\n", 
				thegame.xmoves[0], 
				thegame.xmoves[1], 
				thegame.xmoves[2], 
				thegame.xmoves[3], 
				thegame.xmoves[4]);
		printf("O moves: [%d, %d, %d, %d]\n\n",
				thegame.omoves[0],
				thegame.omoves[1],
				thegame.omoves[2],
				thegame.omoves[3]);
	}
} /* printwinner() */
