/* 
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (c) 2012, 2013 Cyphar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in 
 *    all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>

enum value {
	NONE = 0,
	X = -1,
	O = 1
};

struct field {
	enum value state;
	int position;
};

struct game {
	struct field board[9];
	int players;
	
	int xmoves[5];
	int omoves[4];
	int moves;
	
	bool running;
	bool restart;
	enum value winner;
};

void uholler(char *, ...);
#define holler(...) uholler(__VA_ARGS__, NULL);

void ubail(char *, ...);
#define bail(...) ubail(__VA_ARGS__, NULL);

void bake_args(int, char **);

void initgame(void);
void wargames(void);
void whatside(void);

void cleargame(void);

void makemove(enum value);
void compmove(enum value);

void printboard(bool);
bool checkwinner(void);

void printwinner(void);
void restart(void);

/* Globals */
extern struct game thegame; /* (which you just lost) */
extern enum value player;

extern const char *TTT_VERSION; /* Baked during compile-time */
