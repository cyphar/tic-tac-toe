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
