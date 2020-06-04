#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // make_heap(), push_heap(), pop_heap().
#include <map> // std::multiset (rb-tree)
#include <unordered_map> // std::unordered_multimap (hash-table)
#include <functional>

using namespace std;

class MailBox;
struct Mail;
struct RbElem;
struct HeapElem;
bool heap_comp(HeapElem& hp1, HeapElem& hp2);

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int* date;
	int id;
	int char_count;
	vector<string> keywords;
	// function
	Mail(string _from, string _to, int* _date, int _id, int _char_count, vector<string> _keywords) : from(_from), to(_to), date(_date), id(_id), char_count(_char_count), keywords(_keywords) {}
	void mailInfo();
};

struct RbElem { // Elements of Rb-tree(multiset).
	// variable
	int id;
	int char_count;
	string from;
	int*  date;
	// function
	RbElem(int _id, int _count, string _from, int* _date) : id(_id), char_count(_count), from(_from), date(_date) {}
	void rbElemInfo();
};

struct HeapElem { // Elements of max-heap.
	// variable
	int id;
	int char_count;
	// function
	HeapElem(int _id, int _count) : id(_id), char_count(_count) {}
};


class MailBox { // storage for Mail.

	public:
		void add(string path);
		/* void remove(int target_id); */
		/* void longest(); */
		void heapify() { make_heap(heap.begin(), heap.end(), heap_comp); }
	
	private:
		unordered_multimap<string, Mail> hash_table;
		map<int, RbElem> rb_tree;
		vector<HeapElem> heap;
};

