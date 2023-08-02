CC	:= armv7hl-redhat-linux-gnueabi-gcc
CFLAGS	:= -c

EXE	:= alaw.x
DBG	:= alaw_dbg.x
SOURCES := $(wildcard *.c)
HEADERS	:= $(wildcard *.h)
OBJECTS	:= $(SOURCES:%.c=%.o)
OBJ_DBG	:= $(SOURCES:%.c=%.o)

$(EXE): $(OBJECTS) Makefile
	$(CC) -o $@ $(OBJECTS) 2>&1

$(DBG): $(OBJ_DBG) Makefile
	$(CC) -g -o $@ $(OBJ_DBG) 2>&1

$(OBJECTS): $(SOURCES) $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(SOURCES)

.PHONY: all, debug, clean

all: $(EXE)

debug: CFLAGS += -DDEBUG -g
debug: $(DBG)

clean:
	rm -f -v *~ $(OBJECTS) $(EXE) $(DBG) core
