CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -lncurses -lmenu

SRCDIR = src
OBJDIR = obj
BINDIR = program

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/mvim

all: msee 

msee: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

