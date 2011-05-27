UNAME := $(shell uname -s)
UNAME_OSX := $(shell uname -r)

# I don't like it either...
LIBS := -lldns -I/opt/local/include/mysql5/mysql -I/usr/local/include/mysql++/ -L/opt/local/lib/mysql5/mysql -L/Users/ulissesaraujocosta/work/thoundeyes/mysql++-3.1.0 -lmysqlclient -lmysqlpp_excommon -lmysqlpp

LDNS := ldns
MAIN := Main
RECORD := Record
DATA := Data

OBJS := $(DATA).o $(RECORD).o $(LDNS).o
OUT := out

all:clean $(OUT)

$(OUT):$(OBJS)
ifeq ($(UNAME_OSX),10.7.0)
	g++ -D OS_MACOSX7 $(LIBS) -Wall -o $(OUT) $(OBJS) $(MAIN).cxx
endif

$(RECORD).o: $(RECORD).cxx $(RECORD).hxx
	g++ -c -Wall -o $(RECORD).o $(RECORD).cxx

$(DATA).o: $(DATA).cxx $(DATA).hxx
	g++ -c -I/opt/local/include/mysql5/mysql -I/usr/local/include/mysql++/ -o $(DATA).o $(DATA).cxx

$(LDNS).o: $(LDNS).c $(LDNS).h
	gcc -c -Wall -o $(LDNS).o $(LDNS).c

clean:
	rm -f *.*~ *~ $(OUT) $(OBJS)
