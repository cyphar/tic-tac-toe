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

/* A STRANGE GAME.
 * THE ONLY WINNING MOVE IS NOT TO PLAY */

#include <stdio.h>
#include "../include/ttt.h"

int main(int argc, char *argv[]) {
	bake_args(argc, argv);
	initgame();
	if(thegame.players == 0) wargames();
	do {
		if(thegame.players == 1) whatside();
		if(thegame.players == 2) player = X;
		thegame.restart = false;
		cleargame();
		
		while(thegame.running) {
			if(thegame.players == 1) {
				if(player == X){
					printboard(true);
					makemove(player);
					if(checkwinner()) break;
					compmove(-player);
				} else {
					compmove(-player);
					if(checkwinner()) break;
					printboard(true);
					makemove(player);
				}
			} else {
				printboard(true);
			
				makemove(player);
				player = -player;

			}
			checkwinner();
		}
		
		printwinner();
	} while(thegame.restart);
	return 0;
}
