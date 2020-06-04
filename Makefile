CC:=g++ -std=c++17
obj:=element_jason.o mailbox_jason.o main.o avlTree.o
exe:=jason

.PHONY: all clean

$(exe): $(obj)
	$(CC) $(obj) -o $(exe)

element_jason.o: element_jason.cpp
	$(CC) -c $^ -o $@

mailbox_jason.o: mailbox_jason.cpp
	$(CC) -c $^ -o $@

main.o: main.cpp
	$(CC) -c $^ -o $@

avlTree.o: avlTree.cpp
	$(CC) -c $^ -o $@

clean: 
	rm -rf $(obj) $(exe)
