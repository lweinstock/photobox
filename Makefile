CC=g++
CFLAGS=-Wall -g -Og
LDFLAGS=

# wxwidgets
CFLAGS+=$(shell wx-config --cppflags)
LDFLAGS+=$(shell wx-config --libs)

# libgphoto
CFLAGS+=$(shell pkg-config libgphoto2 --cflags)
LDFLAGS+=$(shell pkg-config libgphoto2 --libs)

# CUPS
CFLAGS+=$(shell pkg-config cups --cflags)
LDFLAGS+=$(shell pkg-config cups --libs)

# opencv
CFLAGS+=$(shell pkg-config opencv --cflags)
LDFLAGS+=$(shell pkg-config opencv --libs)

BIN=photobox
INC=inc
SRC=src

OBJ=
OBJ+=$(SRC)/dslr.o
OBJ+=$(SRC)/printer.o
OBJ+=$(SRC)/webcam.o
OBJ+=$(SRC)/photobox.o
OBJ+=$(SRC)/view.o
OBJ+=$(SRC)/settings.o

.PHONY: all clean hwtests

all: $(BIN)

%.o: %.cpp
	$(CC) -c -o $@ $< -I$(INC) $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

hwtests: hwtests.o $(SRC)/dslr.o $(SRC)/printer.o $(SRC)/webcam.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
