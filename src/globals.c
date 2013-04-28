/*
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (c) 2012, 2013 Cyphar
 *
 * This project is licensed under the MIT/X11 License, see
 * <http://opensource.org/licenses/mit-license> for more details.
 */

#include "ttt.h"

struct game thegame;
enum value player;

#ifndef __TTT_VERSION__
#define __TTT_VERSION__ "n/a"
#endif

const char *TTT_VERSION = __TTT_VERSION__; /* Baked during compile-time */
