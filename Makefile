CC:=g++ -std=c++17
CFLAGS=-O3 -Wall -funroll-loops
obj:= AVLTree.o MailBox_func.o MailBox_main.o 
exe:=run

.PHONY: all clean

all: $(obj)
	$(CC) $(obj) -o $(exe) $(CFLAGS)

AVLTree.o: AVLTree.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

MailBox_func.o: MailBox_func.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

MailBox_main.o: MailBox_main.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

clean: 
	rm -rf $(obj) $(exe)
