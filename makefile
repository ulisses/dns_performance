UNAME := $(shell uname -s)
UNAME_OSX := $(shell uname -r)

LIBS := -lldns

LDNS := ldns
MAIN := Main
RECORD := Record

OBJS := $(RECORD).o $(LDNS).o
OUT := out

$(OUT):$(OBJS)
ifeq ($(UNAME_OSX),10.7.0)
	g++ -D OS_MACOSX7 -lldns -Wall -o $(OUT) $(OBJS) $(MAIN).cxx
endif

$(RECORD).o: $(RECORD).cxx $(RECORD).hxx
	g++ -c -Wall -o $(RECORD).o $(RECORD).cxx

$(LDNS).o: $(LDNS).c $(LDNS).h
	gcc -c -Wall -o $(LDNS).o $(LDNS).c

clean:
	rm -f *.*~ *~ $(OUT) $(OBJS)
