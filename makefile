UNAME := $(shell uname -s)
UNAME_OSX := $(shell uname -r)

LIBS := -lldns

test1:test1.c
ifeq ($(UNAME),Linux)
	g++ $(LDFLAGS) $(FLAGS) $(LIBS) -D OS_LINUX -Wall -o test1 test1.cxx
endif
ifeq ($(UNAME_OSX),10.4.0)
	g++ $(FLAGS) $(LIBS) -D OS_MACOSX_4 -Wall -o test1 test1.cxx
endif
ifeq ($(UNAME_OSX),10.7.0)
	gcc -c -Wall -o test1.o test1.c
	g++ -Wall -o test1 test1.o test1.cxx
endif

clean:
	rm -f *.o *.*~ *~

