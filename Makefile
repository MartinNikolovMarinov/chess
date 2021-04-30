.PHONY: build
build:
	mkdir -p ./build
	g++ main.cpp -o build/main

.PHONY: clean
clean:
	rm -rf ./build
