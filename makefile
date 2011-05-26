UNAME := $(shell uname -s)
UNAME_OSX := $(shell uname -r)

LIBS := -lldns
OUT := test1

$(OUT):$(OUT).c $(OUT).h $(OUT).o
ifeq ($(UNAME),Linux)
	g++ $(LIBS) -D OS_LINUX -Wall -o test1 test1.cxx
endif
ifeq ($(UNAME_OSX),10.4.0)
	g++ $(LIBS) -D OS_MACOSX_4 -Wall -o test1 test1.cxx
endif
ifeq ($(UNAME_OSX),10.7.0)
	g++ -lldns -Wall -o $(OUT) test1.o test1.cxx
endif


$(OUT).o: $(OUT).c $(OUT).h
	gcc -c -Wall -o $(OUT).o $(OUT).c

clean:
	rm -f *.o *.*~ *~ $(OUT)

