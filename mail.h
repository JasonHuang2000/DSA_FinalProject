#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // make_heap(), push_heap(), pop_heap().
#include <set> // std::multiset (rb-tree)
#include <unordered_map> // std::unordered_multimap (hash-table)
#include <functional>
using std::string;

class MailBox;
struct Mail;
struct RbElem;
struct HeapElem;

class MailBox { // storage for Mail.

	public:
		Mailbox() { std::make_heap(heap.begin(), heap.end()); }
		void add(string path);
		void remove(int target_id);
		void longest();
	
	private:
		std::unordered_multimap<string, Mail> hash_table;
		std::multiset<RbElem, std::less<>> rb_tree;
		std::vector<HeapElem> heap;
};

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int date;
	int id;
	int char_count;
	vector<string> content;
	// function
	Mail(string _from, string _to, int _date, int _id) : from(_from), to(_to), date(_date), id(_id);
	void setContent(string readin); // split the original content into clean-words. (also set char_count)
};

struct RbElem { // Elements of Rb-tree(multiset).
	// variable
	int id;
	int char_count;
	string from;
	int date;
	// function
	RbElem(int _id, int _count, string _from, int _date) : id(_id), char_count(_count), from(_from), date(_date);
	bool operator < (const RbElem& rb) { return id < rb.id }
	bool operator > (const RbElem& rb) { return id > rb.id }
};

struct HeapElem { // Elements of max-heap.
	// variable
	int id;
	int char_count;
	// function
	HeapElem(int _id, int _count) : id(_id), char_count(_count);
	bool operator < (const HeapElem& hp) { return char_count < hp.char_count}
	bool operator > (const HeapElem& hp) { return char_count > hp.char_count}
};

