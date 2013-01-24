CC=g++
DEBUG=-g -D_DEBUG
OUTPUTPATH=./
DEFINE=
INCLUDE=
CFLAG=-c -Wall
DFLAG=-MM
RESOURCE=$(wildcard ./src/*.c)
OBJS=$(patsubst %.c, %.o, $(RESOURCE))
DEPENDS=$(patsubst %.c, %.p, $(RESOURCE))
LDFLAG=

Target=../../UniUpdate/Src/Bin/Debug/minilzo.a

.SUFFIXES:.o.p.c
.cpp.o:
	$(CC) $(CFLAG) $(DEBUG) $(INCLUDE) $(DEFINE) $< -o $@
.cpp.p:
	gcc $(DFLAG) $(INCLUDE) $(DEFINE) $< > $@

all: $(Target)

$(Target): $(OBJS) $(DEPENDS)
	ar r $@ $(OBJS)

include $(DEPENDS)

$(DEPENDS): $(RESOURCE)

clean:
	rm -f *.o
	rm -f *.p

test: clean
	echo $(RESOURCE)
	echo $(OBJS)
	echo $(DEPENDS)
	echo $(Target)
