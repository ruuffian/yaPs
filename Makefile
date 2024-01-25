CC:=gcc
TARGET:=bin

IDIR:=include
LDIR:=
ODIR:=obj

WFLAGS:=-Wall -Werror -Wextra -Wno-missing-braces
IFLAGS:=-I$(IDIR)

LIBS:=

_DEPS:=server.h client.h
DEPS:=$(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.c $(DEPS)  
	$(CC) -c -o $@ $< $(WFLAGS) $(IFLAGS)

server: $(ODIR)/server.o
	$(CC) -o $(TARGET)/$@ $< $(WFLAGS) $(IFLAGS) $(LIBS)

client: $(ODIR)/client.o
	$(CC) -o $(TARGET)/$@ $< $(WFLAGS) $(IFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o  

all: server client

.DEFAULT_GOAL:=all
