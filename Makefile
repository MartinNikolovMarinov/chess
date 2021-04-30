CC     = g++
SRC    = main.cpp
CFLAGS = -O0

.PHONY: build
build:
	mkdir -p ./build
	$(CC) $(CFLAGS) $(SRC) -o build/main

.PHONY: clean
clean:
	rm -rf ./build
