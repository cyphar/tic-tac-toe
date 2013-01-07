CC	= gcc
CFLAGS	= -Wall -ansi -O
LFLAGS	=

SRCDIR	= src
HEADDIR	= include

VER	= 1.1
SRC	= $(SRCDIR)/ttt.c $(SRCDIR)/functions.c $(SRCDIR)/globals.c
DEPS	= $(SRCDIR)/ttt.h
EXEC	= tictactoe

$(EXEC): $(SRC) $(DEPS)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LFLAGS) -D__TTT_VERSION__='"$(VER)"'
	strip $(EXEC)

clean: $(EXEC)
	rm $(EXEC)
