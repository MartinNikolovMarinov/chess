CC     = g++
SRC    = main.cpp cutil.cpp display_buf.cpp piece.cpp square.cpp chess_board.cpp
CFLAGS = -O0

.PHONY: build
build:
	mkdir -p ./build
	$(CC) $(CFLAGS) $(SRC) -o build/main

.PHONY: clean
clean:
	rm -rf ./build
