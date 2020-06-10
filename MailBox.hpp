#include <iostream>
#include <string>
#include <vector>
#include <map> // std::multiset (rb-tree)
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

class MailBox;
struct Mail;
struct FromElem;
struct ToElem;

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int* date;
	int id;
	int char_count;
	unordered_set<string> *keywords;
	// function
	Mail(string _from, string _to, int* _date, int _id, int _char_count, unordered_set<string> *_keywords) : from(_from), to(_to), date(_date), id(_id), char_count(_char_count), keywords(_keywords) { }
	void mailInfo();
};

struct FromElem {
	int id;
	string to;
	int* date;	
	FromElem* next;

	FromElem(int id, string to, int* date) : id(id), to(to), date(date), next(NULL) {}
	void insert(FromElem* fe);
	void erase(int id);
};
struct ToElem {
	int id;
	int *date;
	ToElem* next;

	ToElem(int id, int* date) : id(id), date(date), next(NULL) {}
	void insert(ToElem* te);
	void erase(int id);
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
    void erase(T value, int id);
    
    void clear();
    bool empty() const;
    int size() const;
    int find(T value) const;
    const T& find_max() const;
	void longest();
};

class MailBox { // storage for Mail.

	private:
		map<int, Mail> mailSet;
		map<string, FromElem> fromSet;
		map<string, ToElem> toSet;
		AVLTree<int> charCountSet;

	public:
		void add(string& path);
		void remove(int target_id);
		void longest();
		/* void query(string& from, string& to, int* start, int* end, ); */ 
		~MailBox() { 
			mailSet.clear();
			fromSet.clear();
			toSet.clear();
			charCountSet.clear();
		}
};

bool dateComp(int* a, int* b);
