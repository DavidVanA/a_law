CC	:= armv7hl-redhat-linux-gnueabi-gcc

EXE	:= alaw.x
SOURCES := $(wildcard *.c)
HEADERS	:= $(wildcard *.h)
OBJECTS	:= $(SOURCES:%.c=%.o)

$(EXE): $(OBJECTS) Makefile
	$(CC) -o $@ $(OBJECTS) 2>&1

$(OBJECTS): $(SOURCES) $(HEADERS) Makefile
	$(CC) -c $(SOURCES)

.PHONY: all, clean

all: $(EXE)

clean:
	rm -f -v *~ $(OBJECTS) $(EXE) core
