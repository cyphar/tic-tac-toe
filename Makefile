CC	= gcc
CFLAGS	= -Wall --std=c99
LFLAGS	=

SRCDIR	= src
HEADDIR	= include
SRC	= $(SRCDIR)/ttt.c $(SRCDIR)/functions.c $(SRCDIR)/globals.c
DEPS	= $(HEADDIR)/ttt.h
EXEC	= tictactoe

ttt: $(SRC) $(DEPS)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LFLAGS)
	strip $(EXEC)

clean: $(EXEC)
	rm $(EXEC)
