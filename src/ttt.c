/*
 * Tic-Tac-Toe: A heuristic implementation of the classic game.
 * Copyright (c) 2012, 2013 Cyphar
 *
 * This project is licensed under the MIT/X11 License, see
 * <http://opensource.org/licenses/mit-license> for more details.
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
		if(thegame.players == 1)
			whatside();
		else
			player = X;

		thegame.restart = false;
		cleargame();

		while(thegame.running) {
			if(thegame.players == 0) {
				printboard(false);
				compmove(player);

				if(checkwinner())
					break;

				compmove(-player);
			}

			else if(thegame.players == 1) {
				if(player == X){
					printboard(true);
					makemove(player);

					if(checkwinner())
						break;

					compmove(-player);
				}

				else {
					compmove(-player);

					if(checkwinner())
						break;

					printboard(true);
					makemove(player);
				}
			}

			else {
				printboard(true);

				makemove(player);
				player = -player;

			}

			checkwinner();
		}
		printwinner();
		restart();
	} while(thegame.restart);

	/* we so funny */
	if(thegame.players == 0)
		wargames();

	return 0;
}
