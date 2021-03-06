/*
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (C) 2012, 2013 Aleksa Sarai
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
