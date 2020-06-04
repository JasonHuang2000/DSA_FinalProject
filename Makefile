CC:=g++
obj:=element_jason.o mailbox_jason.o main.o
exe:=jason

.PHONY: all clean

$(exe): $(obj)
	$(CC) -std=c++14 -O2 $(obj) -o $(exe)

element_jason.o: element_jason.cpp
	$(CC) -c $^ -o $@

mailbox_jason.o: mailbox_jason.cpp
	$(CC) -c $^ -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -rf $(obj) $(exe)
