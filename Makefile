CC=g++
FLAGS= -Wall -g -O0

all : pattern_matching
	@echo "Compilado corretamente"

pattern_matching : pattern_matching.cpp
	${CC} ${FLAGS} $^ -o $@
