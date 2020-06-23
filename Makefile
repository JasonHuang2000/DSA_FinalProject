CC:=g++ -std=c++17
CFLAGS=-O3 -Wall -funroll-loops
obj:= AVLTree.o MailBox_func.o MailBox_main.o 
exe:=run

.PHONY: all clean

all: 
	$(CC) MailBox_func.cpp MailBox_main.cpp AVLTree.cpp -o $(exe) $(CFLAGS)

clean: 
	rm -rf $(obj) $(exe)
