CC:=gcc
TARGET:=bin

IDIR:=include
LDIR:=
ODIR:=obj

WFLAGS:=-Wall -Werror -Wextra 
IFLAGS:=-I$(IDIR)

LIBS:=

_COMDEPS:=commons.h
COMDEPS:=$(patsubst %,$(IDIR)/%,$(_COMDEPS))

C_DEPS:=$(IDIR)/client.h
S_DEPS:=$(IDIR)/server.h

$(ODIR)/%.o: %.c   
	$(CC) -c -o $@ $< $(WFLAGS) 

server: $(ODIR)/server.o $(S_DEPS) $(COMDEPS)
	$(CC) -o $(TARGET)/$@ $< $(WFLAGS) $(IFLAGS) $(LIBS)

client: $(ODIR)/client.o $(C_DEPS) $(COMDEPS)
	$(CC) -o $(TARGET)/$@ $< $(WFLAGS) $(IFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o  

all: server client

.DEFAULT_GOAL:=all
