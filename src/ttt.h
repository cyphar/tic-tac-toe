/*
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (c) 2012, 2013 Cyphar
 *
 * This project is licensed under the MIT/X11 License, see
 * <http://opensource.org/licenses/mit-license> for more details.
 */

#ifndef __TTT_H__
#define __TTT_H__

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

#endif /* __TTT_H__ */
