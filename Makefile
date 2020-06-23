CC:=g++ -std=c++17 -O3
obj:= AVLTree.o MailBox_func.o MailBox_main.o 
exe:=run

.PHONY: all clean

all: $(obj)
	$(CC) $(obj) -o $(exe)

AVLTree.o: AVLTree.cpp
	$(CC) -c $^ -o $@

MailBox_func.o: MailBox_func.cpp
	$(CC) -c $^ -o $@

MailBox_main.o: MailBox_main_1.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -rf $(obj) $(exe)
