UNAME := $(shell uname -s)
UNAME_OSX := $(shell uname -r)

LIBS := -lldns -lmysqlclient -lmysqlpp_excommon -lmysqlpp

LDNS := ldns
MAIN := Main
RECORD := Record
DATA := Data

OBJS := $(DATA).o $(RECORD).o $(LDNS).o
OUT := dnsperf

all:clean $(OUT)

$(OUT):$(OBJS)
	g++ -D OS_MACOSX7 $(LIBS) -Wall -o $(OUT) $(OBJS) $(MAIN).cxx

$(RECORD).o: $(RECORD).cxx $(RECORD).hxx
	g++ -c -Wall -o $(RECORD).o $(RECORD).cxx

$(DATA).o: $(DATA).cxx $(DATA).hxx
	g++ -c -o $(DATA).o $(DATA).cxx

$(LDNS).o: $(LDNS).c $(LDNS).h
	gcc -c -Wall -o $(LDNS).o $(LDNS).c

clean:
	rm -f *.*~ *~ $(OUT) $(OBJS)
