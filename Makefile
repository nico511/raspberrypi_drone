CC       := gcc
CPP      := g++
CFLAGS   := -c -Wall $(INCLUDE)
INCLUDE  := -I./include/external/
NAME     := ledpwm

all: $(NAME)

clean:
	rm -rf ./obj/*.o ./bin/$(NAME)

$(NAME): $(NAME).o
	$(CPP) ./obj/$(NAME).o -o ./bin/$(NAME)

#$(NAME).o: %.o: ./src/external/%.cpp
#	$(CPP) $< -o $@

$(NAME).o:
	$(CPP) -c ./src/internal/ledpwm.cpp -o ./obj/$(NAME).o
