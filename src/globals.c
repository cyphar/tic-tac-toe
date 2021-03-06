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

#include "ttt.h"

struct game thegame;
enum value player;

#ifndef __TTT_VERSION__
#define __TTT_VERSION__ "n/a"
#endif

const char *TTT_VERSION = __TTT_VERSION__; /* Baked during compile-time */
