CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -lncurses -lmenu

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE = $(BINDIR)/program

all: program

program: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

