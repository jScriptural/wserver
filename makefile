CC := cc
CFLAGS := -Wall -pedantic 

SRC := main.c parser.c address.c bindsock.c routines.c
OBJ := $(SRC:.c=.o)

EXEC := wserver

$(EXEC): $(OBJ) parser.js
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)


clean: 
	rm  $(OBJ)
