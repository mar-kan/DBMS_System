CC = gcc
CFLAFS = -g -Wall

all : HP HT

HP: main_HP.o HP.o HpList.o utilities.o
	$(CC) $(CFLAFS) -o HP main_HP.o HP.o HpList.o utilities.o lib/BF_64.a -no-pie

HT: main_HT.o HT.o utilities.o HtList.o
	$(CC) $(CFLAFS) -o HT main_HT.o HT.o utilities.o HtList.o lib/BF_64.a -no-pie

main_HP.o:
	$(CC) $(CFLAFS) -c examples/main_HP.c

HP.o:
	$(CC) $(CFLAFS) -c src/HP.c

HpList.o:
	$(CC) $(CFLAFS) -c src/HpList.c

HtList.o:
	$(CC) $(CFLAFS) -c src/HtList.c

utilities.o:
	$(CC) $(CFLAFS) -c src/utilities.c

main_HT.o:
	$(CC) $(CFLAFS) -c examples/main_HT.c

HT.o:
	$(CC) $(CFLAFS) -c src/HT.c

clean:
	rm HP HT *.o hash_files/* heap_files/*