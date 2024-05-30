CC=g++
CFLAGS=-Wall -g -Og
LDFLAGS=

# wxwidgets
CFLAGS+=$(shell wx-config --cppflags)
LDFLAGS+=$(shell wx-config --libs)

# libgphoto
CFLAGS+=$(shell pkg-config libgphoto2 --cflags)
LDFLAGS+=$(shell pkg-config libgphoto2 --libs)

# opencv
#CFLAGS+=$(shell pkg-config opencv --cflags)
#LDFLAGS+=$(shell pkg-config opencv --libs)

# CUPS
CFLAGS+=$(shell pkg-config cups --cflags)
LDFLAGS+=$(shell pkg-config cups --libs)

BIN=photobox
INC=inc
SRC=src

OBJ=$(BIN).o
OBJ+=$(SRC)/dslr.o
OBJ+=$(SRC)/printer.o

.PHONY: all clean

all: $(BIN)

%.o: %.cpp
	$(CC) -c -o $@ $< -I$(INC) $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
