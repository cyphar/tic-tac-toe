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

/* A STRANGE GAME.
 * THE ONLY WINNING MOVE IS NOT TO PLAY. */

#include <stdio.h>
#include "ttt.h"

#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	/* Seed randomness. */
	srand(time(NULL) ^ getpid());
	
	bake_args(argc, argv);
	initgame();
	do {
		if(thegame.players == 1) whatside();
		else player = X;
		thegame.restart = false;
		cleargame();
		while(thegame.running) {
			if(thegame.players == 0) {
				printboard(false);
				compmove(player);
				if(checkwinner()) break;
				compmove(-player);
			} else if(thegame.players == 1) {
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
		restart();
	} while(thegame.restart);
	if(thegame.players == 0) wargames();
	return 0;
}
