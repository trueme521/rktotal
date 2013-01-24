CC=g++
DEBUG=-g -D_DEBUG
OUTPUTPATH=./
DEFINE=-DTIXML_USE_STL
INCLUDE=-I../../../Public/Common -ICommon
CFLAG=-c -Wall
DFLAG=-MM
RESOURCE=$(wildcard ./*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(RESOURCE))
DEPENDS=$(patsubst %.cpp, %.p, $(RESOURCE))
LDFLAG=

Target=../../../ShareLib/XmlLib.a

.SUFFIXES:.o.p.cpp
.cpp.o:
	$(CC) $(CFLAG) $(DEBUG) $(INCLUDE) $(DEFINE) $< -o $@
.cpp.p:
	$(CC) $(DFLAG) $(INCLUDE) $(DEFINE) $< > $@

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
