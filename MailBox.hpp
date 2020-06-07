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

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int* date;
	int id;
	int char_count;
	Mail* next;
	vector<string> keywords;
	// function
	Mail(string _from, string _to, int* _date, int _id, int _char_count, vector<string> _keywords) : from(_from), to(_to), date(_date), id(_id), char_count(_char_count), keywords(_keywords) { this->next = NULL; }
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

template <class T>
struct AVLTreeNode {
    AVLTreeNode *left;
    AVLTreeNode *right;
    
    const T value;
	int id;
    int count;  // how many nodes are there in this subtree
    int height;
    
    AVLTreeNode(T value, int id);
    void updateValues();
    int balanceFactor();
    
    AVLTreeNode* left_rotate();
    AVLTreeNode* right_rotate();
};

template <class T>
class AVLTree {
    int _size;
    AVLTreeNode<T> *root;
    
    void balance(std::vector<AVLTreeNode<T>**> path);
    void display(AVLTreeNode<T>* cur, int depth=0, int state=0);
    
public:
    AVLTree();
    ~AVLTree();
    
    void insert(T value, int id);
    void erase(T value);
    
    void clear();
    bool empty() const;
    int size() const;
    
    int find(T value) const;
    int lower_bound(T value) const;
    int upper_bound(T value) const;
    
    const T& find_min() const;
    const T& find_max() const;
    const T& operator[](std::size_t idx) const;
    
	int longest();

    void display();
};

class MailBox { // storage for Mail.

	private:
		/* unordered_map<string, Mail> hash_table; */
		map<int, RbElem> rb_tree;
		AVLTree<int> avl_tree;

	public:
		void add(string path);
		void remove(int target_id);
		void longest();
		~MailBox() { 
			/* hash_table.clear(); */
			rb_tree.clear();
			avl_tree.clear();
		}
};

