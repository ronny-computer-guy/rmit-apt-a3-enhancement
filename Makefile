.default: all

all: ftt

clean:
	rm -rf ftt *.o *.dSYM

ftt: Coin.o Node.o LinkedList.o ftt.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^

valgrind-leak:
	valgrind --leak-check=full --show-leak-kinds=all ./ftt foods.dat coins.dat

valgrind-origins:
	valgrind --track-origins=yes ./ftt foods.dat coins.dat