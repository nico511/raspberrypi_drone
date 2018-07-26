CC       := gcc
CPP      := g++
SRCIN    := ./src/internal
SRCEXT   := ./src/external
OBJDIR   := ./obj
INCLUDE  := -I./include/external -I./include/internal
CFLAGS   := -c -Wall $(INCLUDE)
NAME     := ledpwm

all: $(NAME)

clean:
	rm -rf $(OBJDIR)/*.o ./bin/$(NAME) ./bin/testing

$(NAME): $(NAME).o PCA9685.o I2C.o
	$(CPP) $(OBJDIR)/$(NAME).o $(OBJDIR)/PCA9685.o $(OBJDIR)/I2C.o -o ./bin/$(NAME)

$(NAME).o:
	$(CPP) $(CFLAGS) $(SRCIN)/$(NAME).cpp -o $(OBJDIR)/$(NAME).o

testing: MOTOR.o PCA9685.o I2C.o
	$(CPP) $(OBJDIR)/MOTOR.o $(OBJDIR)/PCA9685.o $(OBJDIR)/I2C.o -o ./bin/testing

MOTOR.o:
	$(CPP) $(CFLAGS) $(SRCIN)/MOTOR.cpp -o $(OBJDIR)/MOTOR.o

PCA9685.o:
	$(CPP) $(CFLAGS) $(SRCEXT)/PCA9685.cpp -o $(OBJDIR)/PCA9685.o

I2C.o:
	$(CPP) $(CFLAGS) $(SRCEXT)/I2C.cpp -o $(OBJDIR)/I2C.o
