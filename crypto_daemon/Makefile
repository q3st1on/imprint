all: main

FILES = $(shell find . -type f -name '*.cpp' -o -name "*.h")

CPP_VERSION=c++20
export GPP_ARGS='-std=$(CPP_VERSION)'

main: $(FILES)
	make -C unit_tests
	make -C hash
	make -C ed25519
	make -C bigint
	g++ -c *.cpp
	g++ *.o hash/*.o ed25519/*.o bigint/*.o unit_tests/*.o unit_tests/tests/*.o unit_tests/tests/hash/*.o -o main
