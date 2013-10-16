/*
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (c) 2012, 2013 Cyphar
 *
 * This project is licensed under the MIT/X11 License, see
 * <http://opensource.org/licenses/mit-license> for more details.
 */

#include <stdio.h>	/* fprintf(), printf() */
#include <stdarg.h>	/* va_list, va_*() */
#include <stdlib.h>	/* exit(), srand(), rand() */
#include <getopt.h>	/* getopt_long() */
#include <ctype.h>	/* tolower() */

#include <stdbool.h>	/* bool	*/
#include <termios.h>	/* getch() stuff */

#include "ttt.h"

#define USAGE	"Usage: %s [-cfnpw] [-hvl]\n", __progname

#define SYNTAX	"Usage: %s [options]\n" \
		"\n" \
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
		"Report bugs to <bugs@cyphar.com>\n", __progname

#define LICENSE	"Tic-Tac-Toe: A heuristic implementation of the classic game.\n" \
		"Copyright (c) 2012, 2013 Cyphar\n" \
		"\n" \
		"Permission is hereby granted, free of charge, to any person obtaining a copy of\n" \
		"this software and associated documentation files (the \"Software\"), to deal in\n" \
		"the Software without restriction, including without limitation the rights to\n" \
		"use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of\n" \
		"the Software, and to permit persons to whom the Software is furnished to do so,\n" \
		"subject to the following conditions:\n" \
		"\n" \
		"1. The above copyright notice and this permission notice shall be included in\n" \
		"   all copies or substantial portions of the Software.\n" \
		"\n" \
		"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n" \
		"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" \
		"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n" \
		"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n" \
		"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n" \
		"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n" \
		"SOFTWARE.\n"

#define WARGAMES		"\e[1;96m"

#define COLOUR_X		"\e[1;31m"
#define COLOUR_O		"\e[1;33m"
#define COLOUR_RESET		"\e[0m"

static bool nocr = false;
static bool colour = true;
static bool numon = true;
static bool classicf = false;
static bool print = false;

static void usage(void){
	extern char *__progname;
	printf(USAGE);
} /* usage() */

static void syntax(void) {
	extern char *__progname;
	printf(SYNTAX);
} /* syntax() */

static void version(void) {
	printf("Tic-Tac-Toe (version %s) by Cyphar\n", TTT_VERSION);
} /* version() */

static void license(void) {
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

static char getch(void) {
	char ch;
	static struct termios old, new;

	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= (~ICANON) | ECHO; /* disable buffered i/o and enable echoing */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */

	ch = getchar();
	tcsetattr(0, TCSANOW, &old); /* reset terminal i/o settings */

	if(ch == 3) {
		printf("\n^C\n");
		exit(2);
	}

	return ch;
} /* getch_() */

static int cyrand(unsigned int min, unsigned int max) {
	if(min == max)
		return min;
	return (int) (1.0 * (max + 1.0) * rand() / (RAND_MAX + min + 0.0));
} /* cyrand() */

static void clearscr(void) {
	printf("\e[1;1H\e[2J");
} /* clearscr() */

static void inflush(void) {
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
	int i;
	for(i = 0; i < 9; i++) {
		thegame.board[i].position = i+1;
		thegame.board[i].state = NONE;
	}

	for(i = 0; i < 4; i++) {
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
			} while((thegame.players = getchar() - '0') == '\n' - '0');
			inflush();
		} else {
			printf("How many players? ");
			thegame.players = getch() - '0';
			if(thegame.players != '\n' - '0') printf("\n");
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
			} while((ch = getchar()) == '\n');
			inflush();
		} else {
			printf("X or O? ");
			ch = getch();
			if(ch != '\n') printf("\n");
		}
		ch = tolower(ch);
	} while(ch != 'x' && ch != 'o');
	player = (ch == 'x') ? X : O;
}

void wargames(void) {
	if(thegame.winner == NONE) {
		holler(	colour ? WARGAMES : "",
			"\n",
			"A STRANGE GAME.\n",
			"THE ONLY WINNING MOVE IS NOT TO PLAY.\n",
			colour ? COLOUR_RESET : ""); /* WarGames */
	} else {
		holler( colour ? WARGAMES : "",
			"\n",
			"A STRANGE GAME.\n",
			"THE ONLY WINNING MOVE IS ... WAIT, WHAT?\n",
			colour ? COLOUR_RESET : "");
	}
	exit(0);
} /* wargames() */

void printboard(bool nums) {

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
	if(!classicf)
		puts("+---+---+---+");
	else
		puts("");

	int cur, i, j;

	for(i = 0; i < 3; i++) {
		if(classicf) printf(" ");
		for(j = 0; j < 3; j++) {
			cur = (i * 3) + j;
			if(thegame.board[cur].state == NONE) {
				if(!classicf) {
					if(nums && numon)
						printf("| %d ", thegame.board[cur].position);
					else
						printf("|   ");
				}

				else {
					if(nums && numon) {
						if(j != 2)
							printf(" %d |", thegame.board[cur].position);
						else
							printf(" %d ", thegame.board[cur].position);
					}

					else {
						if(j != 2)
							printf("   |");
						else
							printf("    ");
					}
				}
			}

			else {
				if(!classicf) {
					if(colour)
						printf("| %s ", (thegame.board[cur].state == X) ?
								COLOUR_X "X" COLOUR_RESET : COLOUR_O "O" COLOUR_RESET);
					else
						printf("| %s ", (thegame.board[cur].state == X) ? "X" : "O");
				}

				else {

					if(j != 2) {
						if(colour)
							printf(" %s |", (thegame.board[cur].state == X) ?
								COLOUR_X "X" COLOUR_RESET : COLOUR_O "O" COLOUR_RESET);
						else
							printf(" %s |", (thegame.board[cur].state == X) ? "X" : "O");
					}

					else {
						if(colour)
							printf(" %s ", (thegame.board[cur].state == X) ?
								COLOUR_X "X" COLOUR_RESET : COLOUR_O "O" COLOUR_RESET);
						else
							printf(" %s ", (thegame.board[cur].state == X) ? "X" : "O");

					}

				}
			}
		}

		if(!classicf){
			puts("|\n+---+---+---+");
		}

		else {
			puts("");
			if(i != 2)
				puts(" ---+---+---");
		}
	}

	if(classicf)
		puts("");
} /* printboard() */

void makemove(enum value player) {
	int move;
	do {
		if(!nocr) {
			do {
				printf("Make your move, %s: ", (player == X) ? "X" : "O");
			} while((move = getchar() - '0') == '\n' - '0');

			inflush();
		}

		else {
			printf("Make your move, %s: ", (player == X) ? "X" : "O");
			move = getch() - '0';

			if(move != '\n' - '0')
				printf("\n");
		}
	} while(move > 9 || move < 1 || thegame.board[move - 1].state != NONE);

	thegame.board[move - 1].state = player;

	if(player == X)
		thegame.xmoves[++thegame.moves / 2] = move;
	else
		thegame.omoves[thegame.moves++ / 2] = move;
} /* makemove() */

static bool equal(int a, int b, int c) {
	/* Basically, if a == b && b == c then a == c */
	return (thegame.board[a].state == thegame.board[b].state &&
			thegame.board[b].state == thegame.board[c].state &&
			thegame.board[a].state != NONE);
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

	/* check for ties */
	else {
		int i;
		for(i = 0; i < 9 && eog; i++)
			if(thegame.board[i].state == NONE)
				eog = false;
	}

	thegame.running = !eog;
	return eog;
} /* checkwinner() */

static int oneaway(int a, int b, int c, enum value player) {
	if(thegame.board[b].state == thegame.board[c].state &&
	   thegame.board[a].state == NONE &&
	   thegame.board[b].state == player)
		return a;

	else if(thegame.board[c].state == thegame.board[a].state &&
			thegame.board[b].state == NONE &&
			thegame.board[c].state == player)
		return b;

	else if(thegame.board[a].state == thegame.board[b].state &&
			thegame.board[c].state == NONE &&
			thegame.board[a].state == player)
		return c;

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

	/* Take inspiration from http://bit.ly/ZRxOCg */

	/* Second player should go for the centre square first, if player one didn't */
	else if((ai == O || (ai == X && thegame.moves > 1)) &&
			thegame.board[4].state == NONE)
				pos = 4;

	/* Corners are a far better bet for the first player's first move than edges or the centre ...
	 * ... and is a far better bet for player two (if the centre has been already taken) */
	else if(thegame.moves <= 1 &&
		(thegame.board[0].state == NONE ||
		thegame.board[2].state == NONE ||
		thegame.board[6].state == NONE ||
		thegame.board[8].state == NONE))
			while(thegame.board[(pos = cyrand(0,4) * 2)].state != NONE && pos != 4);

	/* Block corner-only tactics */

	else if(thegame.board[0].state == -ai &&
		thegame.board[8].state == -ai &&
		(thegame.board[1].state == NONE ||
		 thegame.board[3].state == NONE ||
		 thegame.board[5].state == NONE ||
		 thegame.board[7].state == NONE))
			while(thegame.board[(pos = (cyrand(1,4) * 2) - 1)].state != NONE);

	else if(thegame.board[2].state == -ai &&
		thegame.board[6].state == -ai &&
		(thegame.board[1].state == NONE ||
		 thegame.board[3].state == NONE ||
		 thegame.board[5].state == NONE ||
		 thegame.board[7].state == NONE))
			while(thegame.board[(pos = (cyrand(1,4) * 2) - 1)].state != NONE);

	/* Have a hand in every pie - ensure that AI has made a move in every column ... */
	else if(thegame.board[0].state != ai &&
		thegame.board[3].state != ai &&
		thegame.board[6].state != ai)
			while(thegame.board[(pos = cyrand(0,2) * 3)].state != NONE);

	else if(thegame.board[1].state != ai &&
		thegame.board[4].state != ai &&
		thegame.board[7].state != ai)
			while(thegame.board[(pos = 1 + cyrand(0,2) * 3)].state != NONE);

	else if(thegame.board[2].state != ai &&
		thegame.board[5].state != ai &&
		thegame.board[8].state != ai)
			while(thegame.board[(pos = 2 + cyrand(0,2) * 3)].state != NONE);

	/* ... and every row. */
	else if(thegame.board[0].state != ai &&
		thegame.board[1].state != ai &&
		thegame.board[2].state != ai)
			while(thegame.board[(pos = cyrand(0,2))].state != NONE);

	else if(thegame.board[3].state != ai &&
		thegame.board[4].state != ai &&
		thegame.board[5].state != ai)
			while(thegame.board[(pos = 3 + cyrand(0,2))].state != NONE);

	else if(thegame.board[6].state != ai &&
		thegame.board[7].state != ai &&
		thegame.board[8].state != ai)
			while(thegame.board[(pos = 6 + cyrand(0,2))].state != NONE);

	/* Worst case scenario - take the novice approach */
	else {
		int min = 8, max = 0, i;

		/* Dial down the random range to make it more efficient */
		for(i = 0; i < 9; i++)
			if(thegame.board[i].state == NONE)
				max = i;
		for(i = 8; i >= 0; i--)
			if(thegame.board[i].state == NONE)
				min = i;

		while(thegame.board[(pos = cyrand(min,max))].state != NONE);
	}

	if(thegame.board[pos].state != NONE) {
		/* Houston, we have a problem. */
		compmove(ai);
	}

	else {
		/* My move, poor mortal */
		thegame.board[pos].state = ai;
		if(ai == X)
			thegame.xmoves[++thegame.moves / 2] = pos + 1;
		else
			thegame.omoves[thegame.moves++ / 2] = pos + 1;
	}

}

void restart(void) {
	char yn;
	do {
		if(!nocr) {
			do {
				printf("Do you want to play again? ");
			} while((yn = getchar()) == '\n');
			inflush();
		}

		else {
			printf("Do you want to play again? ");
			yn = getch();

			if(yn != '\n')
				puts("");
		}
		yn = tolower(yn);
	} while(yn != 'y' && yn != 'n');

	if(yn == 'y')
		thegame.restart = true;
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
