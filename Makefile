CC=g++
FLAGS= -Wall -g -O0

all : main
	@echo "Compilado corretamente"

main : pattern_matching.cpp
	${CC} ${FLAGS} $^ -o $@
