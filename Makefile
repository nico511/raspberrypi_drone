CC              := gcc
CPP             := g++
SRCIN           := ./src/internal
SRCEXT          := ./src/external
INCIN           := ./include/internal
INCEXT          := ./include/external
OBJDIR          := ./obj
BUILDDIR        := ./bin
INCLUDE         := -I$(INCIN) -I$(INCEXT)
CFLAGS          := -c -Wall $(INCLUDE)

TESTINGOBJFILES := MOTOR.o PCA9685.o I2C.o functions.o testing.o
TESTING         := $(addprefix $(OBJDIR)/,$(TESTINGOBJFILES))

NAME            := ledpwm

OBJFILES        := $(NAME).o PCA9685.o I2C.o
OBJFILESLIST    := $(addprefix $(OBJDIR)/,$(OBJFILES))

.PHONY          := clean

all: directories testing

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)

directories:
	@mkdir -p $(BUILDDIR) $(OBJDIR)

$(NAME): $(OBJFILESLIST)
	$(CPP) $(OBJFILESLIST) -o $(BUILDDIR)/$(NAME)

testing: $(TESTING)
	$(CPP) $(TESTING) -o $(BUILDDIR)/testing




$(OBJDIR)/testing.o: $(SRCIN)/testing.cpp
	$(CPP) $(CFLAGS) $(SRCIN)/testing.cpp -o $(OBJDIR)/testing.o

$(OBJDIR)/$(NAME).o: $(SRCIN)/$(NAME).cpp
	$(CPP) $(CFLAGS) $(SRCIN)/$(NAME).cpp -o $(OBJDIR)/$(NAME).o

$(OBJDIR)/MOTOR.o: $(SRCIN)/MOTOR.cpp $(INCIN)/MOTOR.h
	$(CPP) $(CFLAGS) $(SRCIN)/MOTOR.cpp -o $(OBJDIR)/MOTOR.o

$(OBJDIR)/PCA9685.o: $(SRCEXT)/PCA9685.cpp $(INCEXT)/PCA9685.h
	$(CPP) $(CFLAGS) $(SRCEXT)/PCA9685.cpp -o $(OBJDIR)/PCA9685.o

$(OBJDIR)/I2C.o: $(SRCEXT)/I2C.cpp $(INCEXT)/I2C.h
	$(CPP) $(CFLAGS) $(SRCEXT)/I2C.cpp -o $(OBJDIR)/I2C.o

$(OBJDIR)/functions.o: $(SRCIN)/functions.cpp $(INCIN)/functions.h
	$(CPP) $(CFLAGS) $(SRCIN)/functions.cpp -o $(OBJDIR)/functions.o
