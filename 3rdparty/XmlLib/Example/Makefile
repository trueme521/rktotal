CC=g++
DEBUG=-o2
OUTPUTPATH=./
DEFINE=-DTIXML_USE_STL
INCLUDE=-I../../../Public/Common -I../../../Public/Common/XMLCommon
CFLAG=-c -Wall
DFLAG=-MM
RESOURCE=$(wildcard ./*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(RESOURCE))
DEPENDS=$(patsubst %.cpp, %.p, $(RESOURCE))
LDFLAG=-ldl ../../../ShareLib/XmlLib.a -lpthread

Target=../../Bin/Debug/Example

.SUFFIXES:.o.p.cpp
.cpp.o:
	$(CC) $(CFLAG) $(DEBUG) $(INCLUDE) $(DEFINE) $< -o $@
.cpp.p:
	gcc $(DFLAG) $(INCLUDE) $(DEFINE) $< > $@

all: $(Target)

$(Target): $(OBJS) $(DEPENDS)
	$(CC) $(OBJS) -o $@ $(LDFLAG)

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
