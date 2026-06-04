CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
EXE     := tema2
SRC     := $(EXE).c

.PHONY: build run clean

build: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

run: build
	./$(EXE)

clean:
	rm -f $(EXE)
